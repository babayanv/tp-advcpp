#include "log/format/LogFormatter.hpp"

#include <iomanip>
#include <ctime>


namespace log
{


LogFormatter::LogFormatter(Mod mod)
    : BaseFormatter(mod)
{
}


std::string LogFormatter::fmt(const std::string& msg, Level lv) noexcept
{
    std::ostringstream oss;

    handleColor(oss, lv);
    handleTime(oss);
    handleWeight(oss, lv);

    oss << msg;
    
    if (is_mod(Mod::COLOR))
    {
        oss << color::RESET;
    }

    return oss.str();
}


void LogFormatter::handleColor(std::ostringstream& oss, Level lv)
{
    if (!is_mod(Mod::COLOR))
    {
        return;
    }

    switch (lv)
    {
    case Level::DEBUG:
        oss << color::CYAN;
        break;

    case Level::INFO:
        oss << color::WHITE;
        break;

    case Level::WARNING:
        oss << color::YELLOW;
        break;

    case Level::ERROR:
        oss << color::RED;
        break;
    }
}


void LogFormatter::handleTime(std::ostringstream& oss)
{
    if (!is_mod(Mod::TIME))
    {
        return;
    }

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    oss << '[' << std::put_time(&tm, "%c") << "] ";
}


void LogFormatter::handleWeight(std::ostringstream& oss, Level lv)
{
    if (!is_mod(Mod::WEIGHT))
    {
        return;
    }

    oss << font::BOLD;

    switch (lv)
    {
    case Level::DEBUG:
        oss << "DEBUG: ";
        break;

    case Level::INFO:
        oss << "INFO: ";
        break;

    case Level::WARNING:
        oss << "WARN: ";
        break;

    case Level::ERROR:
        oss << "ERR: ";
        break;
    }

    oss << font::BOLD_OFF;
}

} // namespace log