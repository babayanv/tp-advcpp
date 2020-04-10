#include "epoll_server/utils/FileDescriptor.hpp"
#include "epoll_server/Exception.hpp"

#include <unistd.h>

#include <iostream>
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
    int fd = m_fd;

    m_fd = -1;

    return fd;
}


bool FileDescriptor::is_opened()
{
    return m_fd != -1;
}

} // namespace utils

} // namespace es
