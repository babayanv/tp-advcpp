#include "data_bank/external_sort/partition.hpp"


namespace data_bank
{


Partition::Partition(const utils::TempFile& src, size_t offset, size_t partition_size)
    : m_src(src)
    , m_offset(offset)
    , m_partition_end(offset + partition_size)
{
}


Partition::Partition(Partition&& other) noexcept
    : m_src(other.m_src)
    , m_offset(other.m_offset)
    , m_partition_end(other.m_partition_end)
    , m_lookup_window(std::move(other.m_lookup_window))
    , m_window_size(other.m_window_size)
    , m_iterator(std::exchange(other.m_iterator, nullptr))
{
}


Partition& Partition::operator=(Partition&& other) noexcept
{
    m_src = other.m_src;
    m_offset = other.m_offset;
    m_partition_end = other.m_partition_end;
    m_lookup_window = std::move(other.m_lookup_window);
    m_window_size = other.m_window_size;
    m_iterator = std::exchange(other.m_iterator, nullptr);

    return *this;
}


bool Partition::set_lookup_window(size_t window_size)
{
    if (m_offset >= m_partition_end)
    {
        return false;
    }

    m_window_size = window_size;

    size_t current_window_size = m_window_size;
    if (m_partition_end - m_offset < m_window_size)
    {
        current_window_size = m_partition_end - m_offset;
    }

    m_lookup_window = m_src.get().memory_map(current_window_size, static_cast<long>(m_offset));
    m_iterator = m_lookup_window.begin();

    m_offset += current_window_size;

    return true;
}


bool Partition::move_iterator(ptrdiff_t offset)
{
    m_iterator += offset;

    if (m_iterator >= m_lookup_window.end())
    {
        return set_lookup_window(m_window_size);
    }

    return true;
}


} // namespace data_bank