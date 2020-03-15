#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP

#include <string>


namespace proc {

constexpr const int PIPE_FAILED = -1;
constexpr const int FORK_FAILED = -1;
constexpr const int CHILD_PROCESS = 0;
constexpr const int EXEC_FAILED = -1;
constexpr const int WRITE_FAILED = -1;
constexpr const int READ_FAILED = -1;
constexpr const int READ_EOF = 0;


class Process
{
public:
    explicit Process(const std::string& path);
    ~Process() noexcept;

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    constexpr bool isReadable() const;

    void closeStdin();
    void close() noexcept;

    void terminate() noexcept;

private:
    int m_p2c_fd[2];
    int m_c2p_fd[2];
    pid_t m_pid;

    bool m_is_readable;

private:
    void initPipes();
    void initAsChild(const std::string& path);
    void initAsParent();
};

} // namespace proc

#endif // PROC_PROCESS_HPP
