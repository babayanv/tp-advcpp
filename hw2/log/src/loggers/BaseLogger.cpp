#include "log/loggers/BaseLogger.hpp"


namespace log
{


BaseLogger::BaseLogger(Level lv, Mod mod) noexcept
    : m_level(lv)
    , m_formatter(mod)
{
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
