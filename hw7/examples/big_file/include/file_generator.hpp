#ifndef FILE_GENERATOR_HPP
#define FILE_GENERATOR_HPP

#include "rng.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <tuple>


size_t get_actual_file_size(size_t file_size, size_t entry_size)
{
    return file_size / entry_size * entry_size;
}


template<class KeyType, class ValueType, class = std::enable_if_t<
        std::is_integral_v<KeyType> && std::is_integral_v<ValueType>>>
std::tuple<KeyType, ValueType, size_t> generate_binary_collection(std::string_view filename, size_t file_size)
{
    constexpr size_t entry_size = sizeof(KeyType) + sizeof(ValueType);

    file_size = get_actual_file_size(file_size, entry_size);
    size_t entry_count = file_size / entry_size;
    size_t picked_entry = RNG<size_t>::roll(0, entry_count - 1);

    KeyType picked_key{};
    ValueType picked_value{};

    std::ofstream ofs(filename.data(), std::ios::out | std::ios::binary);

    for (size_t bytes_written = 0; bytes_written < file_size;)
    {
        KeyType key = RNG<KeyType>::roll(0);
        ValueType value = RNG<ValueType>::roll(0);

        ofs.write(reinterpret_cast<char *>(&key), sizeof(key));
        ofs.write(reinterpret_cast<char *>(&value), sizeof(value));

        if (bytes_written == picked_entry * entry_size)
        {
            picked_key = key;
            picked_value = value;
        }

        std::cout << key << '\n';
        bytes_written += entry_size;
    }

    return std::make_tuple(picked_key, picked_value, file_size);
}


template<class KeyType, class ValueType, class = std::enable_if_t<
        std::is_integral_v<KeyType> && std::is_integral_v<ValueType>>>
std::tuple<KeyType, ValueType, size_t> get_file(std::string_view filename, size_t file_size)
{
    namespace fs = std::filesystem;

    file_size = get_actual_file_size(file_size, sizeof(KeyType) + sizeof(ValueType));

    if (fs::exists(filename) && fs::file_size(filename) == file_size)
    {
        std::ifstream ifs(filename.data(), std::ios::in | std::ios::binary);

        KeyType key{};
        ValueType value{};

        ifs.read(reinterpret_cast<char*>(&key), sizeof(key));
        ifs.read(reinterpret_cast<char*>(&value), sizeof(value));

        return std::make_tuple(key, value, file_size);
    }

    return generate_binary_collection<KeyType, ValueType>(filename, file_size);
}


#endif // FILE_GENERATOR_HPP
