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


inline void init(BaseLoggerPtr logger)
{
    Logger::get_instance().set_global_logger(logger);
}

inline void debug(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->debug(log_msg);
}
inline void info(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->info(log_msg);
}
inline void warn(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->warn(log_msg);
}
inline void error(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->error(log_msg);
}

} // namespace log

#endif // LOGGER_HPP
