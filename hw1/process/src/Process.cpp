#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <sys/wait.h>
#include <iostream>
#include <utility>


namespace proc
{


Process::Process(const std::string& path)
    : m_is_readable(true)
{
    Pipe p2c_fd{};
    Pipe c2p_fd{};
    initPipes(p2c_fd, c2p_fd);

    m_pid = fork();

    if (m_pid < 0)
    {
        throw ProcessError();
    }
    if (m_pid == 0)
    {
        initAsChild(path, p2c_fd, c2p_fd);
    }
    if (m_pid > 0)
    {
        initAsParent(p2c_fd, c2p_fd);
    }
}



Process::Process(Process&& other) noexcept
    : m_p2c_fd{std::exchange(other.m_p2c_fd, -1)}
    , m_c2p_fd{std::exchange(other.m_c2p_fd, -1)}
    , m_pid{std::exchange(other.m_pid, -1)}
    , m_is_readable{std::exchange(other.m_is_readable, false)}
{
}


Process::~Process() noexcept
{
    close();

    try
    {
        terminate();
    }
    catch (const ProcessError& bs)
    {
        std::cerr << bs.what() << std::endl;
    }
}


Process& Process::operator=(Process&& other) noexcept
{
    m_p2c_fd = std::exchange(other.m_p2c_fd, -1);
    m_c2p_fd = std::exchange(other.m_c2p_fd, -1);
    m_pid = std::exchange(other.m_pid, -1);
    m_is_readable = std::exchange(other.m_is_readable, false);

    return *this;
}


size_t Process::write(const void* data, size_t len)
{
    ssize_t bytes_written = ::write(m_p2c_fd, data, len);

    if (bytes_written < 0)
    {
        throw ProcessError();
    }

    return bytes_written;
}


void Process::writeExact(const void* data, size_t len)
{
    size_t bytes_written = 0;

    while (bytes_written != len)
    {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written;

        bytes_written += write(buff_begin, len - bytes_written);
    }
}


size_t Process::read(void* data, size_t len)
{
    ssize_t bytes_read = ::read(m_c2p_fd, data, len);

    if (bytes_read < 0)
    {
        throw ProcessError();
    }

    if (bytes_read == 0)
    {
        m_is_readable = false;
    }

    return bytes_read;
}


void Process::readExact(void* data, size_t len)
{
    size_t bytes_read = 0;

    while (bytes_read != len)
    {
        void* buff_begin = static_cast<char*>(data) + bytes_read;

        bytes_read += read(buff_begin, len - bytes_read);
    }
}


bool Process::isReadable() const
{
    return m_is_readable;
}


void Process::closeStdin()
{
    ::close(m_p2c_fd);
}


void Process::close() noexcept
{
    ::close(m_c2p_fd);
    ::close(m_p2c_fd);

    m_is_readable = false;
}


void Process::terminate()
{
    if (kill(m_pid, SIGTERM) < 0 ||
        waitpid(m_pid, NULL, 0) < 0)
    {
        throw ProcessError();
    }
}


void Process::initPipes(Pipe& fd1, Pipe& fd2)
{
    if (pipe(fd1) < 0)
    {
        throw ProcessError();
    }

    if (pipe(fd2) < 0)
    {
        ::close(fd1[0]);
        ::close(fd1[1]);

        throw ProcessError();
    }
}


void Process::initAsChild(const std::string& path, Pipe& p2c_fd, Pipe& c2p_fd)
{
    dup2(p2c_fd[0], STDIN_FILENO);
    dup2(c2p_fd[1], STDOUT_FILENO);

    ::close(p2c_fd[0]);
    ::close(p2c_fd[1]);
    ::close(c2p_fd[0]);
    ::close(c2p_fd[1]);

    if (execl(path.c_str(), path.c_str(), nullptr) < 0)
    {
        throw ProcessError();
    }
}


void Process::initAsParent(Pipe& p2c_fd, Pipe& c2p_fd)
{
    m_p2c_fd = p2c_fd[1];
    m_c2p_fd = c2p_fd[0];

    ::close(p2c_fd[0]);
    ::close(c2p_fd[1]);
}

} // namespace proc
