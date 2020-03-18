#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP

#include <string>


namespace proc {


constexpr int PIPE_FAILED = -1;
constexpr int EXEC_FAILED = -1;
constexpr int WRITE_FAILED = -1;
constexpr int READ_FAILED = -1;
constexpr int READ_EOF = 0;
constexpr int KILL_FAILED = -1;
constexpr int WAIT_FAILED = -1;


class Process
{
    using Pipe = int[2];

public:
    explicit Process(const std::string& path);
    ~Process() noexcept;

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;

    void closeStdin();
    void close() noexcept;

    void terminate();

private:
    int m_p2c_fd;
    int m_c2p_fd;
    pid_t m_pid;

    bool m_is_readable;

private:
    void initPipes(Pipe& fd1, Pipe& fd2);
    void initAsChild(const std::string& path, Pipe& p2c_fd, Pipe& c2p_fd);
    void initAsParent(Pipe& p2c_fd, Pipe& c2p_fd);
};

} // namespace proc

#endif // PROC_PROCESS_HPP
