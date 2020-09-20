#ifndef DATA_BANK_EXTERNAL_SORT_DATA_FILE_HPP
#define DATA_BANK_EXTERNAL_SORT_DATA_FILE_HPP

#include "data_bank/external_sort/cache_file.hpp"
#include "data_bank/external_sort/partition.hpp"
#include "data_bank/utils/file_descriptor.hpp"
#include "data_bank/utils/mem_mapped_file.hpp"
#include "data_bank/utils/temp_file.hpp"
#include "data_bank/errors.hpp"

#include <fcntl.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>


namespace data_bank
{

template<class ValueType, class = std::enable_if_t<std::is_pod_v<ValueType>>>
class DataFile
{
public:
    using KeyType = CacheFile::KeyType ;
    using KeyToIndex = CacheFile::KeyToIndex;
    using IndexImageContainer = CacheFile::IndexImageContainer;
    using KeyToValue = std::tuple<KeyType, ValueType>;
    using PartitionContainer = std::vector<Partition>;

    static constexpr size_t ENTRY_SIZE = sizeof(KeyType) + sizeof(ValueType);


    DataFile(std::string_view file_path, size_t partition_size, std::string_view cache_file_name)
        : m_fd(::open(file_path.data(), O_RDWR))
        , m_cache_file(cache_file_name)
    {
        if (m_fd < 0)
        {
            throw FileError("open error: ");
        }

        if (partition_size % ENTRY_SIZE != 0)
        {
            partition_size += ENTRY_SIZE - partition_size % ENTRY_SIZE;
        }

        if (m_cache_file.is_relevant(file_path))
        {
            m_index_images = m_cache_file.read_index_images();
        }
        else
        {
            m_cache_file.create();
            m_cache_file.write_filename(file_path);
            external_sort(partition_size);
        }
    }


    std::tuple<ValueType, bool> at(KeyType key)
    {
        auto index_i = find_index(key);
        if (index_i == m_index_images.end())
        {
            return std::make_tuple(ValueType{}, false);
        }

        return binary_search(key, index_i->second);
    }

private:
    utils::FileDescriptor m_fd;
    CacheFile m_cache_file;
    PartitionContainer m_partitions;

    IndexImageContainer m_index_images;
    utils::File m_current_index;

private:
    void external_sort(size_t partition_size)
    {
        utils::TempFile partitions_sorted;
        sort_partitions(partitions_sorted, partition_size);
        merge_partitions(partition_size);
    }


    void sort_partitions(const utils::TempFile& partitions_sorted, size_t partition_size)
    {
        size_t offset = 0;
        size_t file_size = m_fd.size();

        while (offset < file_size)
        {
            if (file_size - offset < partition_size)
            {
                partition_size = file_size - offset;
            }

            auto partition = mmap_partition(partition_size, offset);

            std::map<KeyType, ValueType> partition_data;

            read_from_partition(partition, partition_data);
            write_partition(partitions_sorted, partition_data);

            m_partitions.emplace_back(partitions_sorted, offset, partition_size);

            offset += partition_size;
        }
    }


    void merge_partitions(size_t partition_size)
    {
        const size_t lookup_window_size = partition_size < static_cast<size_t>(10 * utils::PAGE_SIZE) ? partition_size : static_cast<size_t>(10 * utils::PAGE_SIZE);

        for (auto& partition : m_partitions)
        {
            partition.set_lookup_window(lookup_window_size);
        }

        while (!m_partitions.empty())
        {
            generate_index();

            size_t left_to_write = partition_size;

            while (!m_partitions.empty() && left_to_write > 0)
            {
                auto [ key_value, holder_partition ] = find_partitions_current_min();
                if (!holder_partition->move_iterator(ENTRY_SIZE))
                {
                    m_partitions.erase(holder_partition);
                }

                auto& [ key, value ] = key_value;

                m_current_index.write(key);
                m_current_index.write(value);
                m_index_images.back().first = key;

                left_to_write -= ENTRY_SIZE;
            }

            m_cache_file.append_image(m_index_images.back());
        }
    }


    void read_from_partition(const utils::MemMappedFile& partition, std::map<KeyType, ValueType>& dest)
    {
        char* read_ptr = partition.begin();
        while (read_ptr < partition.end())
        {
            KeyType key{};
            std::memcpy(&key, read_ptr, sizeof(key));
            read_ptr += sizeof(key);

            ValueType value{};
            std::memcpy(&value, read_ptr, sizeof(value));
            read_ptr += sizeof(value);

            dest.emplace(key, value);
        }
    }


    void write_partition(const utils::TempFile& dest, const std::map<KeyType, ValueType>& src)
    {
        for (auto& [ key, value ] : src)
        {
            dest.write(key);
            dest.write(value);
        }
    }


    utils::MemMappedFile mmap_partition(size_t partition_size, size_t offset)
    {
        return utils::MemMappedFile(m_fd, partition_size, offset);
    }


    auto find_partitions_current_min()
    {
        using Pair = std::tuple<KeyToValue, PartitionContainer::iterator>;

        std::set<Pair> current_values;

        for (auto i = m_partitions.begin(); i != m_partitions.end(); ++i)
        {
            current_values.emplace(i->current_key_value<KeyType, ValueType>(), i);
        }

        return *current_values.begin();
    }


    IndexImageContainer::iterator find_index(KeyType key)
    {
        for (auto image = m_index_images.begin(); image != m_index_images.end(); ++image)
        {
            if (image->first >= key)
            {
                return image;
            }
        }

        return m_index_images.end();
    }


    std::tuple<ValueType, bool> binary_search(const KeyType& key, std::string_view file_name)
    {
        utils::File file(file_name, O_RDWR);
        auto mmap_file = file.memory_map(file.size(), 0);

        auto begin = mmap_file.begin();
        auto end = mmap_file.end();

        while (begin != end)
        {
            size_t mid_entry_offset = (end - begin) / ENTRY_SIZE / 2;
            auto mid = begin + mid_entry_offset * ENTRY_SIZE;

            KeyType read_key{};
            std::memcpy(&read_key, mid, sizeof(read_key));

            if (key < read_key)
            {
                end = mid;
                continue;
            }
            if (key > read_key)
            {
                begin = mid;
                continue;
            }

            ValueType value{};
            std::memcpy(&value, mid + sizeof(read_key), sizeof(value));

            return std::make_tuple(value, true);
        }

        return std::make_tuple(ValueType{}, false);
    }


    std::string generate_index()
    {
        std::string index_name = "image" + std::to_string(m_index_images.size()) + ".idx";
        m_index_images.emplace_back(0, index_name);

        m_current_index.close();
        m_current_index.open(index_name, O_WRONLY | O_CREAT, 0666);

        return index_name;
    }
};

} // namespace data_bank

#endif // DATA_BANK_EXTERNAL_SORT_DATA_FILE_HPP
