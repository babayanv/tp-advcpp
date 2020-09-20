#ifndef DATA_BANK_EXTERNAL_SORT_CACHE_FILE_HPP
#define DATA_BANK_EXTERNAL_SORT_CACHE_FILE_HPP

#include "data_bank/utils/file.hpp"

#include <vector>
#include <sstream>


namespace data_bank
{


class CacheFile
{
public:
    using KeyType = uint64_t;
    using KeyToIndex = std::pair<KeyType, std::string>;
    using IndexImageContainer = std::vector<KeyToIndex>;

    explicit CacheFile(std::string_view file_name);

    bool is_relevant(std::string_view data_file_name);
    IndexImageContainer read_index_images();

    void create();
    void write_filename(std::string_view file_name);
    void append_image(const KeyToIndex& k2i);

private:
    std::string_view m_file_name;
    std::ostringstream m_oss;

    utils::File m_file;

private:
    std::string_view read_data_file_name(std::string_view sv);
};


} // namespace data_bank

#endif // DATA_BANK_EXTERNAL_SORT_CACHE_FILE_HPP
