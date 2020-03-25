#include "log/Logger.hpp"


namespace log
{


Logger& Logger::get_instance()
{
    static Logger s_instance;
    return s_instance;
}


BaseLoggerPtr Logger::get_global_logger() const noexcept
{
    return m_glob_logger;
}


void Logger::set_global_logger(BaseLoggerPtr logger) noexcept
{
    m_glob_logger = std::move(logger);
}


void init(BaseLoggerPtr logger)
{
    Logger::get_instance().set_global_logger(logger);
}


void debug(const std::string& log_msg)
{
    const auto& logger = Logger::get_instance().get_global_logger();

    logger->debug(log_msg);
    logger->flush();
}


void info(const std::string& log_msg)
{
    const auto& logger = Logger::get_instance().get_global_logger();

    logger->info(log_msg);
    logger->flush();
}


void warn(const std::string& log_msg)
{
    const auto& logger = Logger::get_instance().get_global_logger();

    logger->warn(log_msg);
    logger->flush();
}


void error(const std::string& log_msg)
{
    const auto& logger = Logger::get_instance().get_global_logger();

    logger->error(log_msg);
    logger->flush();
}

} // namespace log
