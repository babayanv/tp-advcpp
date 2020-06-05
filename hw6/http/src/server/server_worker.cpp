#include "http/server/server_worker.hpp"
#include "http/errors.hpp"

#include <arpa/inet.h>

#include <utility>


namespace http
{


ServerWorker::ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, HandlerContext ctx)
    : m_server_fd(server_fd)
    , m_done(done)
    , m_ctx(std::move(ctx))
{
    create_epoll();
    add_epoll(m_server_fd, EPOLLIN | EPOLLEXCLUSIVE);
}


void ServerWorker::run()
{
    constexpr size_t EPOLL_SIZE = 128;
    epoll_event events[EPOLL_SIZE];

    int epoll_timeout = std::min(m_ctx.read_timeout.count(), m_ctx.write_timeout.count());

    while (!m_done)
    {
        int fd_count = epoll_wait(m_epoll_fd, events, EPOLL_SIZE, epoll_timeout);
        if (fd_count < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            throw ServerError("Error waiting epoll: ");
        }
        if (fd_count == 0)
        {
            check_clients_timeout();
            continue;
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
                auto client = m_clients.find(fd);

                if (client == m_clients.end())
                {
                    auto routine = utils::Coroutine::create(&ServerWorker::handle_client, this, fd, events[i]);
                    m_clients.emplace(fd, routine);
                    utils::Coroutine::resume(routine);
                }
                else
                {
                    utils::Coroutine::resume(client->second);
                }
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
    if (!(event.events & EPOLLIN))
    {
        return;
    }

    ClientHandler cl_handler(fd, m_ctx);
    cl_handler.handle();

    m_clients.erase(fd);
}


void ServerWorker::check_clients_timeout()
{
    for (auto& client : m_clients)
    {
        utils::Coroutine::resume(client.second);
    }
}


} // namespace http
