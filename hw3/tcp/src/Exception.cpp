#include "tcp/Exception.hpp"
#include <cstring>


namespace tcp
{


SocketError::SocketError()
    : m_msg(std::strerror(errno))
{
}


SocketError::SocketError(const std::string& what_arg)
    : m_msg(what_arg)
{
    m_msg += std::strerror(errno);
}


SocketError::SocketError(const char* what_arg)
    :m_msg(what_arg)
{
    m_msg += std::strerror(errno);
}


const char* SocketError::what() const noexcept {
    return m_msg.c_str();
}

} // namespace tcp
