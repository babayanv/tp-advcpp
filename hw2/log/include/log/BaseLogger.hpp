#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include "log/Level.hpp"

#include <sstream>


namespace log
{

class BaseLogger
{
public:
    virtual ~BaseLogger() = default;

    inline void debug(const std::string& log_msg) noexcept
    {
        log(log_msg, Level::DEBUG);
    }
    inline void info(const std::string& log_msg) noexcept
    {
        log(log_msg, Level::INFO);
    }
    inline void warn(const std::string& log_msg) noexcept
    {
        log(log_msg, Level::WARNING);
    }
    inline void error(const std::string& log_msg) noexcept
    {
        log(log_msg, Level::ERROR);
    }

    inline void set_level(Level lv) noexcept
    {
        m_level = lv;
    }

    inline Level level() noexcept
    {
        return m_level;
    }

    virtual void flush() noexcept = 0;

private:
    Level m_level;

private:
    void log(const std::string& log_msg, Level lv) noexcept;

    virtual void log_impl(const std::string& log_msg) noexcept = 0;

};

} // namespace log

#endif // BASE_LOGGER_HPP
