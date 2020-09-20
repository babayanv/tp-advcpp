#include "data_bank/utils/file.hpp"
#include "data_bank/errors.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>


namespace data_bank::utils
{


File::File(std::string_view file_name, int flags)
{
    open(file_name, flags);
}


File::File(File&& other) noexcept
    : m_fd(std::move(other.m_fd))
{
}


File& File::operator=(File&& other) noexcept
{
    m_fd = other.m_fd.extract();
    return *this;
}


File& File::operator=(utils::FileDescriptor&& fd)
{
    m_fd = std::move(fd);
    return *this;
}


File::~File()
{
    try
    {
        close();
    }
    catch (const FDError& fde)
    {
        std::cerr << fde.what() << std::endl;
    }
}


void File::open(std::string_view file_name, int flags, int permissions)
{
    close();

    if (permissions == -1)
    {
        m_fd = ::open(file_name.data(), flags);
    }
    else
    {
        m_fd = ::open(file_name.data(), flags, permissions);
    }

    if (m_fd < 0)
    {
        throw FileError("open error: ");
    }
}


void File::close()
{
    m_fd.close();
}


size_t File::write(const void* data, size_t len) const
{
    ssize_t bytes_written = ::write(m_fd, data, len);

    if (bytes_written < 0)
    {
        throw FileError("Error writing to file: ");
    }

    return static_cast<size_t>(bytes_written);
}


void File::write_exact(const void* data, size_t len) const
{
    size_t bytes_written = 0;

    while (bytes_written != len)
    {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written;

        bytes_written += write(buff_begin, len - bytes_written);
    }
}


size_t File::read(void* data, size_t len) const
{
    ssize_t bytes_read = ::read(m_fd, data, len);

    if (bytes_read < 0)
    {
        throw FileError("Error reading from temp file: ");
    }

    return static_cast<size_t>(bytes_read);
}


void File::read_exact(void* data, size_t len) const
{
    size_t bytes_read_total = 0;

    while (bytes_read_total != len)
    {
        void* buff_begin = static_cast<char*>(data) + bytes_read_total;

        size_t bytes_read = read(buff_begin, len - bytes_read_total);

        if (bytes_read == 0)
        {
            throw FileError("Error reading from temp file: eof reached: ");
        }

        bytes_read_total += bytes_read;
    }
}


MemMappedFile File::memory_map(size_t len, long offset) const
{
    return MemMappedFile(m_fd, len, offset);
}


size_t File::size() const
{
    return m_fd.size();
}


} // namespace data_bank::utils
