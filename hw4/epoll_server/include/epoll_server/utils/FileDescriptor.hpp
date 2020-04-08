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
    FileDescriptor(int fd);
    FileDescriptor(FileDescriptor&& other);
    ~FileDescriptor() noexcept;

    FileDescriptor(const FileDescriptor& other) = delete;

    FileDescriptor& operator=(int fd);

    operator int() const;
    void close();
    int extract();

private:
    int m_fd = -1;
};

} // namespace utils

} // namespace es

#endif // FILE_DESCRIPTOR_HPP
