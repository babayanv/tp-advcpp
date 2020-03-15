#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <unistd.h>
#include <sys/wait.h>


namespace proc
{

Process::Process(const std::string& path)
    : m_is_readable(true)
{
    initPipes();

    m_pid = fork();
    switch (m_pid) 
    {
    case FORK_FAILED:
        throw BadProcess();

    case CHILD_PROCESS:
        initAsChild(path);
        break;

    default:
        initAsParent();
        return;
    }
}


Process::~Process()
{
    close();
    terminate();
}


size_t Process::write(const void* data, size_t len)
{
    ssize_t bytes_written = ::write(m_p2c_fd[1], data, len);

    if (bytes_written == WRITE_FAILED)
    {
        throw BadProcess();
    }

    return bytes_written;
}


void Process::writeExact(const void* data, size_t len)
{
    size_t bytes_written = 0;

    while (bytes_written != len) {
        const void* buff_begin = static_cast<const char*>(data) + bytes_written;

        bytes_written += write(buff_begin, len - bytes_written);
    }
}


size_t Process::read(void* data, size_t len)
{
    ssize_t bytes_read = ::read(m_c2p_fd[0], data, len);

    if (bytes_read == READ_FAILED)
    {
        throw BadProcess();
    }

    if (bytes_read == READ_EOF)
    {
        m_is_readable = false;
    }

    return bytes_read;
}


void Process::readExact(void* data, size_t len)
{
    size_t bytes_read = 0;

    while (bytes_read != len) {
        void* buff_begin = static_cast<char*>(data) + bytes_read;

        bytes_read += read(buff_begin, len - bytes_read);
    }
}


constexpr bool Process::isReadable() const
{
    return m_is_readable;
}


void Process::closeStdin()
{
    ::close(m_p2c_fd[1]);
}


void Process::close() noexcept
{
    ::close(m_c2p_fd[0]);
    ::close(m_p2c_fd[1]);

    m_is_readable = false;
}


void Process::terminate() noexcept
{
    kill(m_pid, SIGTERM);
    waitpid(m_pid, NULL, 0);
}


void Process::initPipes()
{
    if (pipe(m_p2c_fd) == PIPE_FAILED)
    {
        throw BadProcess();
    }

    if (pipe(m_c2p_fd) == PIPE_FAILED)
    {
        ::close(m_p2c_fd[0]);
        ::close(m_p2c_fd[1]);

        throw BadProcess();
    }
}


void Process::initAsChild(const std::string& path)
{
    dup2(m_p2c_fd[0], STDIN_FILENO);
    dup2(m_c2p_fd[1], STDOUT_FILENO);

    ::close(m_p2c_fd[0]);
    ::close(m_p2c_fd[1]);
    ::close(m_c2p_fd[0]);
    ::close(m_c2p_fd[1]);

    if (execl(path.c_str(), path.c_str(), nullptr) == EXEC_FAILED)
    {
        throw BadProcess();
    }
}


void Process::initAsParent()
{
    ::close(m_p2c_fd[0]);
    ::close(m_c2p_fd[1]);
}

} // namespace proc
