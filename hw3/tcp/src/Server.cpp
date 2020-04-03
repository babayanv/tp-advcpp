#include "tcp/Server.hpp"
#include "tcp/Exception.hpp"

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>


namespace tcp
{


Server::Server(const std::string& address, uint16_t port, int max_connect)
    : m_opened(false)
{
    open(address, port);
    set_max_connect(max_connect);
}


Server::~Server() noexcept
{
    try
    {
        close();
    }
    catch(const SocketError& se)
    {
        std::cerr << se.what() << std::endl;
    }
}


void Server::open(const std::string& address, uint16_t port, int max_connect)
{
    if (is_opened())
    {
        try
        {
            close();
        }
        catch(const SocketError& se)
        {
            std::cerr << se.what() << std::endl;
        }
    }

    m_sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock_fd < 0)
    {
        throw SocketError("Error creating socket: ");
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = ::htons(port);
    if (::inet_aton(address.c_str(), &serv_addr.sin_addr) == 0)
    {
        close();
        throw SocketError("Invalid ipv4 address: ");
    }

    if (::bind(m_sock_fd,
               reinterpret_cast<sockaddr*>(&serv_addr),
               sizeof(serv_addr)) != 0)
    {
        close();
        throw SocketError("Error binding socket: ");
    }

    m_opened = true;

    set_max_connect(max_connect);
}


void Server::close()
{
    int res = ::close(m_sock_fd);

    m_sock_fd = -1;
    m_opened = false;

    if (res != 0)
    {
        throw SocketError("Error closing socket: ");
    }
}


Connection Server::accept()
{
    sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    int conn_fd = ::accept(m_sock_fd,
                           reinterpret_cast<sockaddr*>(&client_addr),
                           &addr_size);

    if (conn_fd < 0)
    {
        throw SocketError("Socket accept failed: ");
    }

    return Connection(conn_fd, client_addr);
}


void Server::set_max_connect(int max_connect)
{
    if (::listen(m_sock_fd, max_connect) != 0)
    {
        throw SocketError("Socket listen failed: ");
    }
}


bool Server::is_opened()
{
    return m_opened;
}

} // namespace tcp
