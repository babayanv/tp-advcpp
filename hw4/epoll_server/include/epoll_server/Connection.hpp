#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "epoll_server/utils/FileDescriptor.hpp"

#include <netinet/ip.h>
#include <sys/epoll.h>

#include <string>
#include <list>


namespace es
{


class Connection
{
    using EventsCont = std::list<epoll_event>;

public:
    Connection(const std::string& dst_addr, uint16_t dst_port);
    Connection(int sock_fd, const sockaddr_in& sock_info, int epoll_fd = -1);
    Connection(Connection&& other);
    ~Connection() noexcept = default;

    Connection& operator=(Connection&& other);

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;

    void open();
    void connect(const std::string& dst_addr, uint16_t dst_port);
    void close();

    bool is_opened();

    size_t write(const void* data, size_t len);
    size_t read(void* data, size_t len);

    const std::string& addr();
    uint16_t port();

    void register_event(const epoll_event& event);
    const EventsCont& events();
    const epoll_event& recent_event();

    void setReadyToRead();
    void setReadyToWrite();


    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t write(const DataType& data)
    {
        return write(&data, sizeof(DataType));
    }

    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t read(DataType& data)
    {
        return read(&data, sizeof(DataType));
    }

private:
    int m_epoll_fd;
    utils::FileDescriptor m_sock_fd;

    std::string m_dst_addr;
    uint16_t m_dst_port;

    EventsCont m_events;

private:
    void modify_epoll(uint32_t events);
};

} // namespace es

#endif // CONNECTION_HPP
