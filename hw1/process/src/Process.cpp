#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <unistd.h>
#include <sys/wait.h>


namespace proc
{

Process::Process(const std::string& path)
    : m_is_readable(true)
{
    int p2c_fd[2] = {0};
    int c2p_fd[2] = {0};
    initPipes(p2c_fd, c2p_fd);

    m_pid = fork();
    switch (m_pid) 
    {
    case FORK_FAILED:
        throw BadProcess();

    case CHILD_PROCESS:
        initAsChild(path, p2c_fd, c2p_fd);
        break;

    default:
        initAsParent(p2c_fd, c2p_fd);
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
    ssize_t bytes_written = ::write(m_p2c_fd, data, len);

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
    ssize_t bytes_read = ::read(m_c2p_fd, data, len);

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


void Process::terminate() noexcept
{
    kill(m_pid, SIGTERM);
    waitpid(m_pid, NULL, 0);
}


void Process::initPipes(Pipe& fd1, Pipe& fd2)
{
    if (pipe(fd1) == PIPE_FAILED)
    {
        throw BadProcess();
    }

    if (pipe(fd2) == PIPE_FAILED)
    {
        ::close(fd1[0]);
        ::close(fd1[1]);

        throw BadProcess();
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

    if (execl(path.c_str(), path.c_str(), nullptr) == EXEC_FAILED)
    {
        throw BadProcess();
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
