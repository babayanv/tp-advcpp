#include "log/loggers/StdoutLogger.hpp"


namespace log
{


StdoutLogger::StdoutLogger(Level lv, Mod mod)
    : BaseLogger(lv, mod)
{
}


void StdoutLogger::flush() noexcept
{
    std::cout.flush();
}


void StdoutLogger::log_impl(const std::string& log_msg) noexcept
{
    std::cout << log_msg << std::endl;
}


std::shared_ptr<StdoutLogger> create_stdout_logger(Level lv, Mod mod) noexcept
{
    return std::make_shared<StdoutLogger>(lv, mod);
}

} // namespace log
