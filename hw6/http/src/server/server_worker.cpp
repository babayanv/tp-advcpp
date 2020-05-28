#include "http/server/server_worker.hpp"
#include "http/errors.hpp"
#include "http/utils/coroutine.hpp"
#include "http/network/connection.hpp"

#include <arpa/inet.h>

#include <utility>


namespace http
{


ServerWorker::ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, Callback on_request)
    : m_server_fd(server_fd)
    , m_done(done)
    , m_on_request(std::move(on_request))
{
    create_epoll();
    add_epoll(m_server_fd, EPOLLIN | EPOLLEXCLUSIVE);
}


void ServerWorker::run()
{
    constexpr size_t EPOLL_SIZE = 128;
    epoll_event events[EPOLL_SIZE];

    while (!m_done)
    {
        int fd_count = epoll_wait(m_epoll_fd, events, EPOLL_SIZE, -1);
        if (fd_count < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            throw ServerError("Error waiting epoll: ");
        }

        for (int i = 0; i < fd_count; ++i)
        {
            int fd = events[i].data.fd;

            if (fd == m_server_fd)
            {
                accept_clients();
            }
            else
            {
                m_clients.emplace(utils::Coroutine::create_and_run(&ServerWorker::handle_client, this, fd, events[i]));
            }
        }
    }
}


void ServerWorker::create_epoll()
{
    m_epoll_fd = epoll_create(1);
    if (m_epoll_fd < 0)
    {
        throw ServerError("Error creating epoll: ");
    }
}


void ServerWorker::add_epoll(int fd, uint32_t events) const
{
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        throw ServerError("Error adding fd to epoll: ");
    }
}


void ServerWorker::accept_clients()
{
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t addr_size = sizeof(client_addr);

        int conn_fd = ::accept4(m_server_fd,
                                reinterpret_cast<sockaddr*>(&client_addr),
                                &addr_size,
                                SOCK_NONBLOCK);
        if (conn_fd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }

            throw ServerError("Error accepting connection: ");
        }

        add_epoll(conn_fd, EPOLLIN | EPOLLEXCLUSIVE);
    }
}


void ServerWorker::handle_client(int fd, epoll_event event)
{
    if (event.events & EPOLLHUP || event.events & EPOLLERR)
    {
        return;
    }

    network::Connection conn(fd);

    while (true)
    {
        std::string msg = conn.read_all(4096);

        if (msg.empty())
        {
            utils::Coroutine::yield();
            continue;
        }

        network::HttpRequest request = network::build_request(msg);

        network::HttpResponse response = m_on_request(request);

        network::send_response(conn, response);

        auto elem = request.headers.find("Connection");
        if (elem == request.headers.end() || elem->second != "Keep-Alive")
        {
            break;
        }
    }
}


} // namespace http
