#ifndef HTTP_NETWORK_CONNECTION_HPP
#define HTTP_NETWORK_CONNECTION_HPP

#include "http/utils/file_descriptor.hpp"

#include <string>


namespace http::network
{


class Connection
{
public:
    explicit Connection(int sock_fd);
    Connection(Connection&& other) noexcept;

    ~Connection() noexcept = default;

    Connection& operator=(Connection&& other) noexcept;

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;

    void close();
    int extract();

    size_t write(const void* data, size_t len) const;
    void write_exact(const void* data, size_t len) const;
    size_t read(void* data, size_t len);
    std::string read_all(long limit);

    size_t send_file(std::string_view filename);

private:
    utils::FileDescriptor m_sock_fd;
};


} // namespace http::network

#endif // HTTP_NETWORK_CONNECTION_HPP
