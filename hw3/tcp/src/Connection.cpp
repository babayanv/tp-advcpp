#include "tcp/Connection.hpp"
#include "tcp/Exception.hpp"

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <utility>
#include <iostream>


namespace tcp
{


Connection::Connection(const std::string& dst_addr, unsigned short dst_port)
    : m_dst_addr(dst_addr)
    , m_dst_port(dst_port)
    , m_opened(true)
{
    m_sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock_fd < 0)
    {
        throw SocketError("Error creating socket: ");
    }

    try
    {
        connect(m_dst_addr, m_dst_port);
    }
    catch (const SocketError& se)
    {
        close();
        throw se;
    }
}


Connection::Connection(Connection&& other)
    : m_sock_fd(std::exchange(other.m_sock_fd, -1))
    , m_dst_addr(std::move(other.m_dst_addr))
    , m_dst_port(std::exchange(other.m_dst_port, -1))
    , m_opened(std::exchange(other.m_opened, false))
{
}


Connection::~Connection() noexcept
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


Connection& Connection::operator=(Connection&& other)
{
    try
    {
        close();
    }
    catch(const SocketError& se)
    {
        std::cerr << se.what() << std::endl;
    }

    m_sock_fd = std::exchange(other.m_sock_fd, -1);
    m_dst_addr = std::move(other.m_dst_addr);
    m_dst_port = std::exchange(other.m_dst_port, -1);
    m_opened = std::exchange(other.m_opened, false);

    return *this;
}


void Connection::connect(const std::string& dst_addr, unsigned short dst_port)
{
    sockaddr_in dst_sock{};
    dst_sock.sin_family = AF_INET;
    dst_sock.sin_port = ::htons(dst_port);
    dst_sock.sin_addr.s_addr = ::inet_addr(dst_addr.c_str());

    if (::connect(m_sock_fd,
            reinterpret_cast<sockaddr*>(&dst_sock),
            sizeof(dst_sock)) != 0)
    {
        throw SocketError("Connection error: ");
    }

    m_dst_addr = dst_addr;
    m_dst_port = dst_port;
}


void Connection::close()
{
    int res = ::close(m_sock_fd);

    m_sock_fd = -1;
    m_opened = false;

    if (res != 0)
    {
        throw SocketError("Error closing socket: ");
    }
}


size_t Connection::write(const void* data, size_t len)
{
    ssize_t bytes_written = ::write(m_sock_fd, data, len);

    if (bytes_written < 0)
    {
        throw SocketError("Error writing to socket: ");
    }

    return bytes_written;
}


void Connection::writeExact(const void* data, size_t len)
{
    size_t bytes_written = 0;

    while (bytes_written != len)
    {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written;

        bytes_written += write(buff_begin, len - bytes_written);
    }
}


size_t Connection::read(void* data, size_t len)
{
    ssize_t bytes_read = ::read(m_sock_fd, data, len);

    if (bytes_read < 0)
    {
        throw SocketError("Error reading from socket: ");
    }

    return bytes_read;
}


void Connection::readExact(void* data, size_t len)
{
    size_t bytes_read_total = 0;

    while (bytes_read_total != len)
    {
        void* buff_begin = static_cast<char*>(data) + bytes_read_total;

        size_t bytes_read = read(buff_begin, len - bytes_read);

        if (bytes_read == 0)
        {
            m_opened = false;
            throw SocketError("Socket file descriptor is closed: ");
        }

        bytes_read_total += bytes_read;
    }
}


void Connection::set_receive_timeout(int timeout_sec)
{
    timeval timeout{timeout_sec, 0};

    if (::setsockopt(m_sock_fd,
            SOL_SOCKET,
            SO_RCVTIMEO,
            &timeout,
            sizeof(timeout)) != 0)
    {
        throw SocketError("Error setting socket options: ");
    }
}


void Connection::set_send_timeout(int timeout_sec)
{
    timeval timeout{timeout_sec, 0};

    if (::setsockopt(m_sock_fd,
            SOL_SOCKET,
            SO_SNDTIMEO,
            &timeout,
            sizeof(timeout)) != 0)
    {
        throw SocketError("Error setting socket options: ");
    }
}


bool Connection::is_opened()
{
    return m_opened;
}


const std::string& Connection::get_addr()
{
    return m_dst_addr;
}


uint16_t Connection::get_port()
{
    return m_dst_port;
}



Connection::Connection(int sock_fd, const sockaddr_in& sock_info)
    : m_sock_fd(std::exchange(sock_fd, -1))
    , m_dst_addr(15, '\0')
    , m_dst_port(sock_info.sin_port)
    , m_opened(true)
{
    if (inet_ntop(AF_INET,
                  &sock_info.sin_addr,
                  m_dst_addr.data(),
                  static_cast<in_port_t>(m_dst_addr.size())) == nullptr)
    {
        close();
        throw SocketError("Error converting accepted address from binary to string: ");
    }
}

} // namespace tcp
