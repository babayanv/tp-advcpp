#ifndef DATA_BANK_UTILS_FILE_DESCRIPTOR_HPP
#define DATA_BANK_UTILS_FILE_DESCRIPTOR_HPP

#include <cstddef>


namespace data_bank::utils
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
    FileDescriptor& operator=(FileDescriptor&& other);

    operator int() const noexcept;
    void close();
    int extract();

    bool is_opened() const noexcept;

    size_t size() const;

private:
    int m_fd = -1;
};

} // namespace data_bank::utils

#endif // DATA_BANK_UTILS_FILE_DESCRIPTOR_HPP