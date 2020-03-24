#include "log/loggers/BaseLogger.hpp"


namespace log
{


BaseLogger::BaseLogger(Level lv, Mod mod) noexcept
    : m_level(lv)
    , m_formatter(mod)
{
}


void BaseLogger::debug(const std::string& log_msg) noexcept
{
    log(log_msg, Level::DEBUG);
}


void BaseLogger::info(const std::string& log_msg) noexcept
{
    log(log_msg, Level::INFO);
}


void BaseLogger::warn(const std::string& log_msg) noexcept
{
    log(log_msg, Level::WARNING);
}


void BaseLogger::error(const std::string& log_msg) noexcept
{
    log(log_msg, Level::ERROR);
}


void BaseLogger::set_level(Level lv) noexcept
{
    m_level = lv;
}


Level BaseLogger::level() noexcept
{
    return m_level;
}


void BaseLogger::log(const std::string& log_msg, Level lv) noexcept
{
    if (lv > m_level)
    {
        return;
    }

    log_impl(m_formatter.fmt(log_msg, lv));
}

} //  namespace log
