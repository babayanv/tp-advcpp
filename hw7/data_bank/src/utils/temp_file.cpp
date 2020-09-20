#include "data_bank/utils/temp_file.hpp"
#include "data_bank/errors.hpp"

#include <unistd.h>


namespace data_bank::utils
{


TempFile::TempFile()
{
    std::string m_name{"XXXXXX"};

    File::m_fd = ::mkstemp(m_name.data());
    ::unlink(m_name.c_str());
}


TempFile::TempFile(TempFile&& other) noexcept
{
    File::m_fd = std::move(other.m_fd);
}


TempFile& TempFile::operator=(TempFile&& other) noexcept
{
    m_fd = other.m_fd.extract();

    return *this;
}


} // data_bank::utils
