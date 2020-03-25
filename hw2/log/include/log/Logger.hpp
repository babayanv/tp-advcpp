#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "log/loggers/BaseLogger.hpp"

#include <memory>


namespace log
{


using BaseLoggerPtr = std::shared_ptr<BaseLogger>;

class Logger
{
public:
    static Logger& get_instance();

    BaseLoggerPtr get_global_logger() const noexcept;
    void set_global_logger(BaseLoggerPtr logger) noexcept;

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

} // namespace log

#endif // LOGGER_HPP
