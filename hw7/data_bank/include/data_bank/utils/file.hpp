#ifndef DATA_BANK_UTILS_FILE_HPP
#define DATA_BANK_UTILS_FILE_HPP

#include "data_bank/utils/file_descriptor.hpp"
#include "data_bank/utils/mem_mapped_file.hpp"

#include <cstddef>
#include <type_traits>
#include <string_view>


namespace data_bank::utils
{


class File
{
public:
    File() = default;
    File(std::string_view file_name, int flags);
    File(File&& other) noexcept;
    File& operator=(File&& other) noexcept;
    File& operator=(utils::FileDescriptor&& fd);

    virtual ~File();

    void open(std::string_view file_name, int flags, int permissions = -1);
    void close();

    size_t write(const void* data, size_t len) const;
    void write_exact(const void* data, size_t len) const;

    template<class DataType, class = std::enable_if_t<std::is_pod_v<DataType>>>
    void write(const DataType& data) const
    {
        write_exact(&data, sizeof(data));
    }


    size_t read(void* data, size_t len) const;
    void read_exact(void* data, size_t len) const;

    template<class DataType, class = std::enable_if_t<std::is_pod_v<DataType>>>
    void read(DataType& data) const
    {
        read_exact(&data, sizeof(DataType));
    }


    MemMappedFile memory_map(size_t len, long offset) const;

    size_t size() const;

protected:
    utils::FileDescriptor m_fd;
};


} // namespace data_bank::utils

#endif // DATA_BANK_UTILS_FILE_HPP
