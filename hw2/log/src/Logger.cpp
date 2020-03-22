#include "log/Logger.hpp"


namespace log
{


Logger& Logger::get_instance()
{
    static Logger s_instance;
    return s_instance;
}


void init(BaseLoggerPtr logger)
{
    Logger::get_instance().set_global_logger(logger);
}


void debug(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->debug(log_msg);
}


void info(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->info(log_msg);
}


void warn(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->warn(log_msg);
}


void error(const std::string& log_msg)
{
    Logger::get_instance().get_global_logger()->error(log_msg);
}


void flush()
{
    Logger::get_instance().get_global_logger()->flush();
}

} // namespace log
