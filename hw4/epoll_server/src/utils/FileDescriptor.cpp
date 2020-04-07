#include "epoll_server/utils/FileDescriptor.hpp"

#include <unistd.h>

#include <utility>


namespace es
{

namespace utils
{


FileDescriptor::FileDescriptor(int fd)
    : m_fd(fd)
{
}


FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : m_fd(std::exchange(other.m_fd, -1))
{
}


FileDescriptor::~FileDescriptor() noexcept
{
    close();
}


FileDescriptor& FileDescriptor::operator=(int fd)
{
    close();

    m_fd = fd;

    return *this;
}


FileDescriptor::operator int() const
{
    return m_fd;
}


void FileDescriptor::close()
{
    if (m_fd == -1)
    {
        return;
    }

    ::close(m_fd);

    m_fd = -1;
}


int FileDescriptor::extract()
{
    int fd = m_fd;

    m_fd = -1;

    return fd;
}

} // namespace utils

} // namespace es
