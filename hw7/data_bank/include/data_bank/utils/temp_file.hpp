#ifndef DATA_BANK_UTILS_TEMP_FILE_HPP
#define DATA_BANK_UTILS_TEMP_FILE_HPP

#include "data_bank/utils/file.hpp"

#include <string_view>
#include <string>


namespace data_bank::utils
{


class TempFile : public File
{
public:
    TempFile();
    TempFile(TempFile&& other) noexcept;
    TempFile& operator=(TempFile&& other) noexcept;
};


} // data_bank::utils

#endif // DATA_BANK_UTILS_TEMP_FILE_HPP