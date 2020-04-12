#include "epoll_server/Server.hpp"
#include "epoll_server/Exception.hpp"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>


namespace es
{


Server::Server(const std::string& address, uint16_t port, int max_connect, Callback&& do_handle_client)
    : m_connections(max_connect)
    , m_do_handle_client(std::forward<Callback>(do_handle_client))
{
    try
    {
        open(address, port);
        listen(max_connect);
        create_epoll();
        add_epoll(m_sock_fd, EPOLLIN);
    }
    catch (const ServerError& se)
    {
        close();
        throw se;
    }
}


Server::Server(uint16_t port, int max_connect, Callback&& do_handle_client)
    : Server("0.0.0.0", port, max_connect,
             std::forward<Callback>(do_handle_client))
{
}


void Server::init(const std::string& address, uint16_t port, int max_connect, Callback&& do_handle_client)
{
    if (is_opened())
    {
        close();
    }

    m_do_handle_client = std::forward<Callback>(do_handle_client);

    open(address, port);
    listen(max_connect);
    create_epoll();
    add_epoll(m_sock_fd, EPOLLIN);
}


void Server::init(uint16_t port, int max_connect, Callback&& do_handle_client)
{
    init("0.0.0.0", port, max_connect,
          std::forward<Callback>(do_handle_client));
}


void Server::open(const std::string& address, uint16_t port)
{
    if (is_opened())
    {
        close();
    }

    m_sock_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_sock_fd < 0)
    {
        throw ServerError("Error creating socket: ");
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = ::htons(port);
    if (::inet_aton(address.c_str(), &serv_addr.sin_addr) == 0)
    {
        throw ServerError("Invalid ipv4 address: ");
    }

    if (::bind(m_sock_fd,
               reinterpret_cast<sockaddr*>(&serv_addr),
               sizeof(serv_addr)) != 0)
    {
        throw ServerError("Error binding socket: ");
    }
}


void Server::open(uint16_t port)
{
    open("0.0.0.0", port);
}


void Server::listen(int max_connect) const
{
    if (::listen(m_sock_fd, max_connect) != 0)
    {
        throw ServerError("Socket listen failed: ");
    }
}


void Server::close()
{
    m_sock_fd.close();
    m_epoll_fd.close();
}


bool Server::is_opened() const noexcept
{
    return m_sock_fd.is_opened();
}


void Server::run()
{
    constexpr size_t EPOLL_SIZE = 128;
    epoll_event events[EPOLL_SIZE];

    while (is_opened())
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

            if (fd == m_sock_fd)
            {
                accept_clients();
            }
            else
            {
                handle_client(fd, events[i]);
            }
        }
    }
}


void Server::create_epoll()
{
    m_epoll_fd = epoll_create(1);
    if (m_epoll_fd < 0)
    {
        throw ServerError("Error creating epoll: ");
    }
}


void Server::add_epoll(int fd, uint32_t events) const
{
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        throw ServerError("Error adding fd to epoll: ");
    }
}


void Server::accept_clients()
{
    while (true)
    {
        sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int conn_fd = ::accept4(m_sock_fd,
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

        m_connections.emplace(std::piecewise_construct,
                              std::forward_as_tuple(conn_fd),
                              std::forward_as_tuple(conn_fd, client_addr, m_epoll_fd));

        add_epoll(conn_fd, EPOLLIN | EPOLLRDHUP);
    }
}


void Server::handle_client(int fd, epoll_event event)
{
    Connection& conn = m_connections.at(fd);
    conn.register_event(event);

    m_do_handle_client(conn);

    if (event.events & EPOLLHUP ||
        event.events & EPOLLERR ||
        event.events & EPOLLRDHUP)
    {
        m_connections.erase(fd);
    }
}

} // namespace es
