#include "http/network/connection.hpp"
#include "http/errors.hpp"

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>

#include <sstream>
#include <filesystem>


namespace http::network
{


Connection::Connection(int sock_fd)
        : m_sock_fd(sock_fd)
{
}


Connection::Connection(Connection&& other) noexcept
        : m_sock_fd(other.m_sock_fd.extract())
{
}


Connection& Connection::operator=(Connection&& other) noexcept
{
    close();
    m_sock_fd = other.m_sock_fd.extract();
    return *this;
}


void Connection::close()
{
    m_sock_fd.close();
}


int Connection::extract()
{
    return m_sock_fd.extract();
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
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EPIPE)
            {
                return 0;
            }

            throw ConnectionError("Error writing to socket: ");
        }

        return static_cast<size_t>(bytes_written);
    }
}


void Connection::write_exact(const void* data, size_t len) const
{
    size_t bytes_written_total = 0;

    while (bytes_written_total != len)
    {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written_total;

        size_t bytes_written = write(buff_begin, len - bytes_written_total);
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

        return static_cast<size_t>(bytes_read);
    }
}


std::string Connection::read_all(long limit)
{
    std::ostringstream oss;

    while (oss.tellp() < limit)
    {
        std::string chunk(1024, '\0');

        size_t bytes_read = read(chunk.data(), chunk.size());

        oss.write(chunk.c_str(), static_cast<long>(bytes_read));

        if (bytes_read < chunk.size())
        {
            break;
        }
    }

    return oss.str();
}


size_t Connection::send_file(std::string_view file_path)
{
    namespace fs = std::filesystem;

    utils::FileDescriptor file_fd = ::open(file_path.data(), O_RDONLY);
    if (file_fd < 0)
    {
        throw ConnectionError("Error sending file to socket: ");
    }

    off_t read_offset = 0;
    size_t bytes_left = fs::file_size(file_path);
    size_t bytes_written_total = 0;

    do
    {
        ssize_t bytes_written = ::sendfile(m_sock_fd, file_fd, &read_offset, bytes_left);

        if (bytes_written < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EPIPE)
            {
                return bytes_written_total;
            }

            throw ConnectionError("Error sending file to socket: ");
        }

        bytes_written_total += static_cast<size_t>(bytes_written);
        bytes_left -= static_cast<size_t>(bytes_written);
    }
    while (bytes_left > 0);

    return bytes_written_total;
}


} // namespace http::network
