#include "log/BaseLogger.hpp"

#include <iomanip>


namespace log
{


void BaseLogger::debug(const std::string& log_msg) noexcept
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream os;
    os << color::CYAN <<
        '[' << std::put_time(&tm, "%c") << "] " <<
        font::BOLD << "DEBUG: " << font::BOLD_OFF <<
        log_msg << color::DEFAULT << std::endl;

    log(os.str(), Level::DEBUG);
}


void BaseLogger::info(const std::string& log_msg) noexcept
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream os;
    os << color::WHITE <<
        '[' << std::put_time(&tm, "%c") << "] " <<
        font::BOLD << "INFO: " << font::BOLD_OFF <<
        log_msg << color::DEFAULT << std::endl;

    log(os.str(), Level::INFO);
}


void BaseLogger::warn(const std::string& log_msg) noexcept
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream os;
    os << color::YELLOW <<
        '[' << std::put_time(&tm, "%c") << "] " <<
        font::BOLD << "WARN: " << font::BOLD_OFF <<
        log_msg << color::DEFAULT << std::endl;

    log(os.str(), Level::WARNING);
}


void BaseLogger::error(const std::string& log_msg) noexcept
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream os;
    os << color::RED <<
        '[' << std::put_time(&tm, "%c") << "] " <<
        font::BOLD << "ERR: " << font::BOLD_OFF <<
        log_msg << color::DEFAULT << std::endl;

    log(os.str(), Level::ERROR);
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

    log_impl(log_msg);
}

} //  namespace log
