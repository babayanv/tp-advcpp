#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/ip.h>

#include <string>


namespace tcp
{


class Connection
{
    friend class Server;
public:
    Connection(const std::string& address, unsigned short port);
    Connection(const Connection& other) = delete;
    Connection(Connection&& other);

    ~Connection() noexcept;

    Connection& operator=(const Connection& other) = delete;
    Connection& operator=(Connection&& other);

    void connect(const std::string& address, unsigned short port);
    void close();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    void set_receive_timeout(int timeout_sec);
    void set_send_timeout(int timeout_sec);
    bool is_opened();

    const std::string& get_addr();
    uint16_t get_port();


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
    int m_sock_fd;

    std::string m_dst_addr;
    uint16_t m_dst_port;

    bool m_opened;

private:
    Connection(int sock_fd, const sockaddr_in& sock_info);
};

} // namespace tcp

#endif // CONNECTION_HPP
