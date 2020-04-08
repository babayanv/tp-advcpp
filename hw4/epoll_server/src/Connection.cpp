#include "epoll_server/Connection.hpp"
#include "epoll_server/Exception.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <utility>


namespace es
{


Connection::Connection(const std::string& dst_addr, uint16_t dst_port)
    : m_dst_addr(dst_addr)
    , m_dst_port(dst_port)
{
    open();

    try
    {
        connect(m_dst_addr, m_dst_port);
    }
    catch (const ConnectionError& ce)
    {
        close();
        throw ce;
    }
}


Connection::Connection(int sock_fd, const sockaddr_in& sock_info)
    : m_sock_fd(sock_fd)
    , m_dst_addr(15, '\0')
    , m_dst_port(sock_info.sin_port)
{
    if (inet_ntop(AF_INET,
                  &sock_info.sin_addr,
                  m_dst_addr.data(),
                  static_cast<in_port_t>(m_dst_addr.size())) == nullptr)
    {
        close();
        throw ConnectionError("Error converting accepted address from binary to string: ");
    }
}


Connection::Connection(Connection&& other)
    : m_sock_fd(other.m_sock_fd.extract())
    , m_dst_addr(std::move(other.m_dst_addr))
    , m_dst_port(std::exchange(other.m_dst_port, -1))
{
}


Connection& Connection::operator=(Connection&& other)
{
    close();

    m_sock_fd = other.m_sock_fd.extract();
    m_dst_addr = std::move(other.m_dst_addr);
    m_dst_port = std::exchange(other.m_dst_port, -1);

    return *this;
}


void Connection::open()
{
    if (is_opened())
    {
        close();
    }

    m_sock_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_sock_fd < 0)
    {
        throw ConnectionError("Error creating socket: ");
    }
}


void Connection::connect(const std::string& dst_addr, uint16_t dst_port)
{
    sockaddr_in dst_sock{};
    dst_sock.sin_family = AF_INET;
    dst_sock.sin_port = ::htons(dst_port);
    dst_sock.sin_addr.s_addr = ::inet_addr(dst_addr.c_str());

    if (::connect(m_sock_fd,
                  reinterpret_cast<sockaddr*>(&dst_sock),
                  sizeof(dst_sock)) != 0)
    {
        if (errno != EINPROGRESS)
        {
            throw ConnectionError("Connection error: ");
        }
    }

    m_dst_addr = dst_addr;
    m_dst_port = dst_port;
}


void Connection::close()
{
    m_sock_fd.close();
}


size_t Connection::write(const void* data, size_t len)
{
    ssize_t bytes_written = ::write(m_sock_fd, data, len);

    if (bytes_written < 0)
    {
        throw ConnectionError("Error writing to socket: ");
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
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }

        throw ConnectionError("Error reading from socket: ");
    }

    if (bytes_read == 0)
    {
        close();
    }

    return bytes_read;
}


void Connection::readExact(void* data, size_t len)
{
    size_t bytes_read_total = 0;

    while (bytes_read_total != len)
    {
        void* buff_begin = static_cast<char*>(data) + bytes_read_total;

        size_t bytes_read = read(buff_begin, len - bytes_read_total);

        if (bytes_read == 0)
        {
            throw ConnectionError("Unable to read exactly " + std::to_string(len) + " bytes: ");
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
        throw ConnectionError("Error setting socket options: ");
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
        throw ConnectionError("Error setting socket options: ");
    }
}


const std::string& Connection::addr()
{
    return m_dst_addr;
}


uint16_t Connection::port()
{
    return m_dst_port;
}


bool Connection::is_opened()
{
    return m_sock_fd != -1;
}


void Connection::register_event(const epoll_event& event)
{
    m_events.emplace_front(event);
}


const Connection::EventsCont& Connection::events()
{
    return m_events;
}


const epoll_event& Connection::recent_event()
{
    return m_events.front();
}

} // namespace es
