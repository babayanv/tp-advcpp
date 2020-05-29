#include "http/server/server.hpp"
#include "http/errors.hpp"

#include <arpa/inet.h>


namespace http
{


Server::Server(std::string_view address, uint16_t port, size_t max_conn)
{
    try
    {
        open(address, port);
        listen(static_cast<int>(max_conn));
    }
    catch (const ServerError& se)
    {
        close();
        throw;
    }
}


Server::~Server() noexcept
{
    join_threads();
}


void Server::open(std::string_view address, uint16_t port)
{
    if (is_opened())
    {
        close();
    }

    m_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_fd < 0)
    {
        throw ServerError("Error creating socket: ");
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = ::htons(port);
    if (::inet_aton(address.data(), &serv_addr.sin_addr) == 0)
    {
        throw ServerError("Invalid ipv4 address: ");
    }

    if (::bind(m_fd,
               reinterpret_cast<sockaddr*>(&serv_addr),
               sizeof(serv_addr)) != 0)
    {
        throw ServerError("Error binding socket: ");
    }
}


void Server::listen(int max_connect) const
{
    if (::listen(m_fd, max_connect) != 0)
    {
        throw ServerError("Socket listen failed: ");
    }
}


void Server::close()
{
    m_fd.close();
}


bool Server::is_opened() const noexcept
{
    return m_fd.is_opened();
}


void Server::run(size_t thread_limit, TimeoutType read_timeout, TimeoutType write_timeout)
{
    m_done = false;

    m_threads.reserve(thread_limit);

    for (size_t i = 0; i < thread_limit - 1; ++i)
    {
        m_threads.emplace_back(
            [this, read_timeout, write_timeout]
            {
                ServerWorker sw(m_fd, m_done, read_timeout, write_timeout,
                        [this](const network::HttpRequest& request, SendFileCallback& enqueue_send_file)
                        {
                            return on_request(request, enqueue_send_file);
                        });
                sw.run();
            });
    }

    ServerWorker sw(m_fd, m_done, read_timeout, write_timeout,
            [this](const network::HttpRequest& request, SendFileCallback& enqueue_send_file)
            {
                return on_request(request, enqueue_send_file);
            });
    sw.run();
}


void Server::join_threads()
{
    if (m_done)
    {
        return;
    }

    m_done = true;

    for (auto& i : m_threads)
    {
        i.join();
    }
}


void Server::handle_signal()
{
    join_threads();
    close();
}


} // namespace http
