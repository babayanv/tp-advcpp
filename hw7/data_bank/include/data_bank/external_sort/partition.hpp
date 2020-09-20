#ifndef DATA_BANK_EXTERNAL_SORT_PARTITION_HPP
#define DATA_BANK_EXTERNAL_SORT_PARTITION_HPP

#include "data_bank/utils/mem_mapped_file.hpp"
#include "data_bank/utils/temp_file.hpp"

#include <cstring>
#include <tuple>
#include <functional>


namespace data_bank
{


class Partition
{
public:
    Partition(const utils::TempFile& src, size_t offset, size_t partition_size);
    Partition(Partition&& other) noexcept;
    Partition& operator=(Partition&& other) noexcept;

    bool set_lookup_window(size_t window_size);

    bool move_iterator(ptrdiff_t offset);


    template <class KeyType, class ValueType, class = std::enable_if_t<std::is_pod_v<KeyType> && std::is_pod_v<ValueType>>>
    std::tuple<KeyType, ValueType> current_key_value()
    {
        KeyType key{};
        std::memcpy(&key, m_iterator, sizeof(key));

        ValueType value{};
        std::memcpy(&value, m_iterator + sizeof(KeyType), sizeof(value));

        return std::make_tuple(key, value);
    }

private:
    std::reference_wrapper<const utils::TempFile> m_src;

    size_t m_offset;
    size_t m_partition_end;

    utils::MemMappedFile m_lookup_window;
    size_t m_window_size{};
    char* m_iterator{};
};


} // namespace data_bank

#endif // DATA_BANK_EXTERNAL_SORT_PARTITION_HPP
