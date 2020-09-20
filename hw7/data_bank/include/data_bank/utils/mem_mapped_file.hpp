#ifndef DATA_BANK_UTILS_MEM_MAPPED_FILE_HPP
#define DATA_BANK_UTILS_MEM_MAPPED_FILE_HPP

#include "data_bank/utils/file_descriptor.hpp"

#include <zconf.h>


namespace data_bank::utils
{


const long PAGE_SIZE = ::sysconf(_SC_PAGESIZE);


class MemMappedFile
{
public:
    MemMappedFile() = default;
    MemMappedFile(int fd, size_t len, long offset);
    MemMappedFile(MemMappedFile&& other) noexcept ;
    MemMappedFile& operator=(MemMappedFile&& other) noexcept;

    ~MemMappedFile();

    MemMappedFile(const MemMappedFile& other) = delete;
    MemMappedFile& operator=(const MemMappedFile& other) = delete;

    operator char*();

    void load(int fd, size_t len, long offset);
    void unmap();

    char* begin() const;
    char* end() const;

private:
    char* m_mmap_ptr{};
    char* m_begin{};
    size_t m_len{};

private:
    long page_align_offset_lower(long& value) const;
    size_t page_align_len_upper(size_t& value) const;
};


} // data_bank::utils

#endif // DATA_BANK_UTILS_MEM_MAPPED_FILE_HPP
