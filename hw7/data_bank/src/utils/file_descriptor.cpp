#include "data_bank/utils/file_descriptor.hpp"
#include "data_bank/errors.hpp"

#include <unistd.h>
#include <sys/stat.h>

#include <utility>
#include <iostream>


namespace data_bank::utils
{


FileDescriptor::FileDescriptor(int fd) noexcept
        : m_fd(fd)
{
}


FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept
        : m_fd(std::exchange(other.m_fd, -1))
{
}


FileDescriptor::~FileDescriptor() noexcept
{
    try
    {
        close();
    }
    catch(const FDError& fde)
    {
        std::cerr << fde.what() << std::endl;
    }
}


FileDescriptor& FileDescriptor::operator=(int fd)
{
    close();
    m_fd = fd;
    return *this;
}


FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other)
{
    close();
    m_fd = std::exchange(other.m_fd, -1);
    return *this;
}


FileDescriptor::operator int() const noexcept
{
    return m_fd;
}


void FileDescriptor::close()
{
    if (m_fd == -1)
    {
        return;
    }

    while (::close(m_fd) != 0)
    {
        if (errno == EINTR)
        {
            continue;
        }

        throw FDError("Error closing file descriptor: ");
    }

    m_fd = -1;
}


int FileDescriptor::extract()
{
    return std::exchange(m_fd, -1);
}


bool FileDescriptor::is_opened() const noexcept
{
    return m_fd != -1;
}


size_t FileDescriptor::size() const
{
    struct stat stat_buf{};
    if (::fstat(m_fd, &stat_buf) < 0)
    {
        throw FileError("fstat error: ");
    }

    return static_cast<size_t>(stat_buf.st_size);
}


} // namespace data_bank::utils