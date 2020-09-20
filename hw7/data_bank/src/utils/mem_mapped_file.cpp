#include "data_bank/utils/mem_mapped_file.hpp"
#include "data_bank/errors.hpp"

#include <sys/mman.h>

#include <utility>
#include <iostream>


namespace data_bank::utils
{


MemMappedFile::MemMappedFile(int fd, size_t len, long offset)
{
    load(fd, len, offset);
}


MemMappedFile::MemMappedFile(MemMappedFile&& other) noexcept
    : m_mmap_ptr(std::exchange(other.m_mmap_ptr, nullptr))
    , m_begin(std::exchange(other.m_begin, nullptr))
    , m_len(other.m_len)
{
}


MemMappedFile& MemMappedFile::operator=(MemMappedFile&& other) noexcept
{
    m_mmap_ptr = std::exchange(other.m_mmap_ptr, nullptr);
    m_begin = std::exchange(other.m_begin, nullptr);
    m_len = other.m_len;

    return *this;
}


MemMappedFile::~MemMappedFile()
{
    try
    {
        unmap();
    }
    catch (const FileError& fe)
    {
        std::cout << fe.what() << std::endl;
    }
}


MemMappedFile::operator char*()
{
    return m_begin;
}


void MemMappedFile::load(int fd, size_t len, long offset)
{
    unmap();

    long offset_diff = page_align_offset_lower(offset);

    auto mmap_ptr = static_cast<char*>(::mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset));
    if (mmap_ptr == MAP_FAILED)
    {
        throw FileError("mmap error: ");
    }

    m_begin = mmap_ptr + offset_diff;
    m_len = len;
}


void MemMappedFile::unmap()
{
    if (m_mmap_ptr == nullptr)
    {
        return;
    }

    size_t actual_length = m_len;
    page_align_len_upper(actual_length);
    if (::munmap(m_mmap_ptr, actual_length) < 0)
    {
        throw FileError("munmap error: ");
    }

    m_mmap_ptr = nullptr;
    m_len = 0;
}


char* MemMappedFile::begin() const
{
    return m_begin;
}


char* MemMappedFile::end() const
{
    return m_begin + m_len;
}


long MemMappedFile::page_align_offset_lower(long& value) const
{
    long value_diff = value - (value / PAGE_SIZE) * PAGE_SIZE;

    value -= value_diff;

    return value_diff;
}


size_t MemMappedFile::page_align_len_upper(size_t& value) const
{
    size_t value_diff = (1 + value / static_cast<unsigned long>(PAGE_SIZE)) * static_cast<unsigned long>(PAGE_SIZE) - value;

    value += value_diff;

    return value_diff;
}


} // namespace data_bank::utils
