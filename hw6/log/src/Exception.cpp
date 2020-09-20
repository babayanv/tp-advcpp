#include "log/Exception.hpp"


namespace log
{


LoggerError::LoggerError(const std::string& what_arg)
    : m_msg(what_arg)
{
}


LoggerError::LoggerError(const char* what_arg)
    :m_msg(what_arg)
{
}


const char* LoggerError::what() const noexcept {
    return m_msg.c_str();
}


LoggerThreadStopped::LoggerThreadStopped(const std::string& what_arg)
    : LoggerError(what_arg)
{
}


LoggerThreadStopped::LoggerThreadStopped(const char* what_arg)
    : LoggerError(what_arg)
{
}


} // namespace log