#include "log/Logger.hpp"


namespace log
{


Logger& Logger::get_instance()
{
    static Logger s_instance;
    return s_instance;
}


BaseLoggerPtr Logger::get_global_logger()
{
    return m_glob_logger;
}


void Logger::set_global_logger(BaseLoggerPtr logger)
{
    m_glob_logger = std::move(logger);
}


void init(BaseLoggerPtr logger)
{
    Logger::get_instance().set_global_logger(logger);
}


void debug(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->debug(log_msg);
    Logger::get_instance().get_global_logger()->flush();
}


void info(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->info(log_msg);
    Logger::get_instance().get_global_logger()->flush();
}


void warn(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->warn(log_msg);
    Logger::get_instance().get_global_logger()->flush();
}


void error(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->error(log_msg);
    Logger::get_instance().get_global_logger()->flush();
}

} // namespace log
