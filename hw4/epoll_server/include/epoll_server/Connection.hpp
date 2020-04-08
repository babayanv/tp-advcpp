#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "epoll_server/utils/FileDescriptor.hpp"

#include <netinet/ip.h>
#include <sys/epoll.h>

#include <string>
#include <forward_list>


namespace es
{


class Connection
{
    using EventsCont = std::forward_list<epoll_event>;

public:
    Connection(const std::string& dst_addr, uint16_t dst_port);
    Connection(int sock_fd, const sockaddr_in& sock_info);
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
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    void set_receive_timeout(int timeout_sec);
    void set_send_timeout(int timeout_sec);

    const std::string& addr();
    uint16_t port();

    void register_event(const epoll_event& event);
    const EventsCont& events();
    const epoll_event& recent_event();


    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    void write(const DataType& data)
    {
        writeExact(&data, sizeof(DataType));
    }

    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    void read(DataType& data)
    {
        readExact(&data, sizeof(DataType));
    }

private:
    utils::FileDescriptor m_sock_fd;

    std::string m_dst_addr;
    uint16_t m_dst_port;

    EventsCont m_events;
};

} // namespace es

#endif // CONNECTION_HPP
