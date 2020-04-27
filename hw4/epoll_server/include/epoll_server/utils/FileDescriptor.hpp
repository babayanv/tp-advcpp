#ifndef FILE_DESCRIPTOR_HPP
#define FILE_DESCRIPTOR_HPP


namespace es
{

namespace utils
{


class FileDescriptor
{
public:
    FileDescriptor() = default;
    FileDescriptor(int fd) noexcept;
    FileDescriptor(FileDescriptor&& other) noexcept;
    ~FileDescriptor() noexcept;

    FileDescriptor(const FileDescriptor& other) = delete;

    FileDescriptor& operator=(int fd);

    operator int() const noexcept;
    void close();
    int extract();

    bool is_opened() const noexcept;

private:
    int m_fd = -1;
};

} // namespace utils

} // namespace es

#endif // FILE_DESCRIPTOR_HPP
