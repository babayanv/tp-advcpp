#ifndef HTTP_UTILS_FILE_DESCRIPTOR_HPP
#define HTTP_UTILS_FILE_DESCRIPTOR_HPP


namespace http::utils
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

} // namespace http::utils

#endif // HTTP_UTILS_FILE_DESCRIPTOR_HPP
