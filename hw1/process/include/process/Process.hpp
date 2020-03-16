#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP

#include <string>


namespace proc {

constexpr int PIPE_FAILED = -1;
constexpr int FORK_FAILED = -1;
constexpr int CHILD_PROCESS = 0;
constexpr int EXEC_FAILED = -1;
constexpr int WRITE_FAILED = -1;
constexpr int READ_FAILED = -1;
constexpr int READ_EOF = 0;


class Process
{
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

    void terminate() noexcept;

private:
    int m_p2c_fd;
    int m_c2p_fd;
    pid_t m_pid;

    bool m_is_readable;

private:
    void initPipes(int (&fd1)[2], int (&fd2)[2]);
    void initAsChild(const std::string& path, int (&p2c_fd)[2], int (&c2p_fd)[2]);
    void initAsParent(int (&p2c_fd)[2], int (&c2p_fd)[2]);
};

} // namespace proc

#endif // PROC_PROCESS_HPP
