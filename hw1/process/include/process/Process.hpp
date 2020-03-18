#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP

#include <string>
#include <unistd.h>



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

    void terminate();


    template<class DataType,
        class = typename std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t write(const DataType& data);

    template<class DataType,
        class = typename std::enable_if_t<std::is_pod_v<DataType>>
    >
    void writeExact(const DataType& data);

    template<class DataType,
        class = typename std::enable_if_t<std::is_pod_v<DataType>>
    >
    size_t read(DataType& data);

    template<class DataType,
        class = typename std::enable_if_t<std::is_pod_v<DataType>>
    >
    void readExact(DataType& data);


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


#include "Process.inl"

} // namespace proc

#endif // PROC_PROCESS_HPP
