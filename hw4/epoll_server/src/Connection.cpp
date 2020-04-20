#include "epoll_server/Connection.hpp"
#include "epoll_server/Exception.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <utility>


namespace es
{


Connection::Connection(const std::string& dst_addr, uint16_t dst_port, int epoll_fd)
    : m_epoll_fd(epoll_fd)
    , m_dst_addr(dst_addr)
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


Connection::Connection(int sock_fd, int epoll_fd)
    : m_epoll_fd(epoll_fd)
    , m_sock_fd(sock_fd)
    , m_dst_addr(15, '\0')
{
    sockaddr_in sock_info{};
    socklen_t sock_len = sizeof(sock_info);
    if (getsockname(m_sock_fd,
                    reinterpret_cast<sockaddr*>(&sock_info),
                    &sock_len) < 0)
    {
        close();
        throw ConnectionError("Error getting socket info: ");
    }

    m_dst_addr = inet_ntoa(sock_info.sin_addr);
    m_dst_port = ntohs(sock_info.sin_port);
}


Connection::Connection(int sock_fd, const sockaddr_in& sock_info, int epoll_fd)
    : m_epoll_fd(epoll_fd)
    , m_sock_fd(sock_fd)
    , m_dst_addr(15, '\0')
    , m_dst_port(ntohs(sock_info.sin_port))
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


Connection::Connection(Connection&& other) noexcept
    : m_epoll_fd(std::exchange(other.m_epoll_fd, -1))
    , m_sock_fd(other.m_sock_fd.extract())
    , m_dst_addr(std::move(other.m_dst_addr))
    , m_dst_port(std::exchange(other.m_dst_port, -1))
{
}


Connection& Connection::operator=(Connection&& other)
{
    close();

    m_epoll_fd = std::exchange(other.m_epoll_fd, -1);
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


size_t Connection::write(const void* data, size_t len) const
{
    while (true)
    {
        ssize_t bytes_written = ::write(m_sock_fd, data, len);
        if (bytes_written < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return 0;
            }

            throw ConnectionError("Error writing to socket: ");
        }

        return bytes_written;
    }
}


void Connection::write_exact(const void* data, size_t len) const
{
    size_t bytes_written_total = 0;

    while (bytes_written_total != len)
    {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written_total;

        ssize_t bytes_written = write(buff_begin, len - bytes_written_total);
        if (bytes_written == 0)
        {
            throw ConnectionError("Unable to write exactly " + std::to_string(len) + " bytes: ");
        }

        bytes_written_total += bytes_written;
    }
}


size_t Connection::read(void* data, size_t len)
{
    while (true)
    {
        ssize_t bytes_read = ::read(m_sock_fd, data, len);

        if (bytes_read < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return 0;
            }

            throw ConnectionError("Error reading from socket: ");
        }

        if (bytes_read == 0)
        {
            close();
            return 0;
        }

        m_input_registry.emplace_back(static_cast<char*>(data), bytes_read);

        return bytes_read;
    }
}


size_t Connection::input_bytes() const noexcept
{
    size_t bytes{};

    for (auto& i : m_input_registry)
    {
        bytes += i.size();
    }

    return bytes;
}


std::string Connection::input_cat() const
{
    std::string str;

    for (auto& i : m_input_registry)
    {
        str += i;
    }

    return str;
}


void Connection::input_clear() noexcept
{
    m_input_registry.clear();
}


void Connection::read_exact(void* data, size_t len)
{
    size_t bytes_read_total = 0;

    while (bytes_read_total < len)
    {
        void* buff_begin = static_cast<char*>(data) + bytes_read_total;

        ssize_t bytes_read = read(buff_begin, len - bytes_read_total);
        if (bytes_read == 0)
        {
            throw ConnectionError("Unable to read exactly " + std::to_string(len) + " bytes: ");
        }

        bytes_read_total += bytes_read;
    }
}


const std::string& Connection::addr() const noexcept
{
    return m_dst_addr;
}


uint16_t Connection::port() const noexcept
{
    return m_dst_port;
}


bool Connection::is_opened() const noexcept
{
    return m_sock_fd.is_opened();
}


void Connection::register_event(const epoll_event& event)
{
    m_events_registry.emplace_front(event);
}


const Connection::EventsRegistry& Connection::events() const noexcept
{
    return m_events_registry;
}


const epoll_event& Connection::recent_event() const noexcept
{
    return m_events_registry.front();
}


const Connection::InputRegistry& Connection::input() const noexcept
{
    return m_input_registry;
}


void Connection::setReadyToRead() const
{
    modify_epoll(EPOLLIN | EPOLLRDHUP);
}


void Connection::setReadyToWrite() const
{
    modify_epoll(EPOLLOUT);
}


void Connection::modify_epoll(uint32_t events) const
{
    epoll_event event{};
    event.data.fd = m_sock_fd;
    event.events = events;

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, m_sock_fd, &event) < 0)
    {
        throw ServerError("Error modifying fd events: ");
    }
}

} // namespace es
