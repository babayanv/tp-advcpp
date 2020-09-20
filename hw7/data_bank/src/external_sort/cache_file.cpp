#include "data_bank/external_sort/cache_file.hpp"
#include "data_bank/errors.hpp"

#include <fcntl.h>
#include <sys/stat.h>

#include <filesystem>
#include <charconv>
#include <string>


namespace data_bank
{


CacheFile::CacheFile(std::string_view file_name)
    : m_file_name(file_name)
{
}


bool CacheFile::is_relevant(std::string_view data_file_name)
{
    namespace fs = std::filesystem;
    if (!fs::exists(m_file_name))
    {
        return false;
    }

    m_file.open(m_file_name, O_RDWR);
    auto mmap_file = m_file.memory_map(m_file.size(), 0);
    std::string_view sv(mmap_file);

    std::string_view cache_data_file_name = read_data_file_name(sv);

    return !(cache_data_file_name != data_file_name);
}


CacheFile::IndexImageContainer CacheFile::read_index_images()
{
    auto mmap_file = m_file.memory_map(m_file.size(), 0);
    std::string_view sv(mmap_file);

    read_data_file_name(sv);

    size_t pos{};
    IndexImageContainer index_file_names;

    do
    {
        KeyType key{};
        pos = sv.find('_');
        std::from_chars(sv.begin(), sv.begin() + pos, key);
        sv.remove_prefix(pos + 1);

        pos = sv.find(';');
        std::string_view index(sv.substr(0, pos));
        sv.remove_prefix(pos + 1);

        index_file_names.emplace_back(key, index);
    }
    while(pos != std::string_view::npos);

    return index_file_names;
}


void CacheFile::create()
{
    m_file.open(m_file_name, O_WRONLY | O_CREAT, 0666);
}


void CacheFile::write_filename(std::string_view file_name)
{
    std::string str(file_name.data());
    str += '\n';
    m_file.write(str.c_str(), str.size());
}


void CacheFile::append_image(const KeyToIndex& k2i)
{
    auto& [ key, index_name ] = k2i;

    std::ostringstream oss;
    oss << key << '_' << index_name << ';';

    std::string str(oss.str());
    m_file.write(str.c_str(), str.size());
}


std::string_view CacheFile::read_data_file_name(std::string_view sv)
{
    auto pos = sv.find('\n');
    if (pos == std::string_view::npos)
    {
        throw CacheFileError("Invalid cache file");
    }

    auto data_file_name = sv.substr(0, pos);
    sv.remove_prefix(pos + 1);

    return data_file_name;
}


} // namespace data_bank