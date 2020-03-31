#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/ip.h>

#include <string>


namespace tcp
{


class Connection
{
public:
    Connection(int&& sock_fd, const sockaddr_in& sock_info);
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
    unsigned short m_dst_port;

    bool m_opened;
};

} // namespace tcp

#endif // CONNECTION_HPP
