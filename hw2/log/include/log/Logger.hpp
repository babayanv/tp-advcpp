#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "log/BaseLogger.hpp"

#include <memory>


namespace log
{


using BaseLoggerPtr = std::shared_ptr<BaseLogger>;

class Logger
{
public:
    static Logger& get_instance();

    inline BaseLoggerPtr get_global_logger()
    {
        return m_glob_logger;
    }

    inline void set_global_logger(BaseLoggerPtr logger)
    {
        m_glob_logger = logger;
    }

private:
    BaseLoggerPtr m_glob_logger;

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
};


void init(BaseLoggerPtr logger);

void debug(const std::string& log_msg);
void info(const std::string& log_msg);
void warn(const std::string& log_msg);
void error(const std::string& log_msg);

void flush();

} // namespace log

#endif // LOGGER_HPP