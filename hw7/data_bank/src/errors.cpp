#include "data_bank/errors.hpp"

#include <cstring>


namespace data_bank
{


ErrnoException::ErrnoException()
        : m_msg(std::strerror(errno))
{
}


ErrnoException::ErrnoException(std::string  what_arg)
        : m_msg(std::move(what_arg))
{
    m_msg += std::strerror(errno);
}


const char* ErrnoException::what() const noexcept
{
    return m_msg.c_str();
}


CacheFileError::CacheFileError(std::string what_arg)
    : m_msg(what_arg)
{
}


const char* CacheFileError::what() const noexcept
{
    return m_msg.c_str();
}


} // namespace data_bank