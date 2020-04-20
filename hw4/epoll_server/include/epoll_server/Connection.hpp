#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "epoll_server/utils/FileDescriptor.hpp"

#include <netinet/ip.h>
#include <sys/epoll.h>

#include <string>
#include <list>
#include <vector>


namespace es
{


class Connection
{
    using EventsRegistry = std::list<epoll_event>;
    using InputRegistry = std::vector<std::string>;

public:
    Connection(const std::string& dst_addr, uint16_t dst_port, int epoll_fd = -1);
    Connection(int sock_fd, int epoll_fd = -1);
    Connection(int sock_fd, const sockaddr_in& sock_info, int epoll_fd = -1);
    Connection(Connection&& other) noexcept;
    ~Connection() noexcept = default;

    Connection& operator=(Connection&& other);

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;

    void open();
    void connect(const std::string& dst_addr, uint16_t dst_port);
    void close();

    bool is_opened() const noexcept;

    size_t write(const void* data, size_t len) const;
    void write_exact(const void* data, size_t len) const;
    size_t read(void* data, size_t len);
    void read_exact(void* data, size_t len);

    const std::string& addr() const noexcept;
    uint16_t port() const noexcept;

    void register_event(const epoll_event& event);
    const EventsRegistry& events() const noexcept;
    const epoll_event& recent_event() const noexcept;

    const InputRegistry& input() const noexcept;
    size_t input_bytes() const noexcept;
    std::string input_cat() const;
    void input_clear() noexcept;

    void setReadyToRead() const;
    void setReadyToWrite() const;


    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t write(const DataType& data) const
    {
        return write_exact(&data, sizeof(DataType));
    }

    template<class DataType,
        class = std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t read(DataType& data)
    {
        return read_exact(&data, sizeof(DataType));
    }

private:
    int m_epoll_fd;
    utils::FileDescriptor m_sock_fd;

    std::string m_dst_addr;
    uint16_t m_dst_port;

    EventsRegistry m_events_registry;
    InputRegistry m_input_registry;

private:
    void modify_epoll(uint32_t events) const;
};

} // namespace es

#endif // CONNECTION_HPP
