#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP

#include <string>


namespace proc {

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

    void terminate() noexcept;

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
