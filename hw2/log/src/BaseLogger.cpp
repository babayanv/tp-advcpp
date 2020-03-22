#include "log/BaseLogger.hpp"


namespace log
{

void BaseLogger::log(const std::string& log_msg, Level lv) noexcept
{
    if (lv > m_level)
    {
        return;
    }

    log_impl(log_msg);
}

} //  namespace log
