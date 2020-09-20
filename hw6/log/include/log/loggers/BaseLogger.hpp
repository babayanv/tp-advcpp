#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include "log/Properties.hpp"
#include "log/format/LogFormatter.hpp"

#include <sstream>


namespace log
{

class BaseLogger
{
public:
    explicit BaseLogger(Level lv, Mod mod) noexcept;
    virtual ~BaseLogger() = default;

    void debug(const std::string& log_msg) noexcept;
    void info(const std::string& log_msg) noexcept;
    void warn(const std::string& log_msg) noexcept;
    void error(const std::string& log_msg) noexcept;

    void set_level(Level lv) noexcept;
    Level level() noexcept;

    virtual void flush() noexcept = 0;

private:
    Level m_level;
    LogFormatter m_formatter;

private:
    void log(const std::string& log_msg, Level lv) noexcept;

    virtual void log_impl(const std::string& log_msg) noexcept = 0;

};

} // namespace log

#endif // BASE_LOGGER_HPP
