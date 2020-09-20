#include "log/loggers/StderrLogger.hpp"


namespace log
{


StderrLogger::StderrLogger(Level lv, Mod mod)
    : BaseLogger(lv, mod)
{
}


void StderrLogger::flush() noexcept
{
    std::cerr.flush();
}


void StderrLogger::log_impl(const std::string& log_msg) noexcept
{
    std::cerr << log_msg << std::endl;
}


std::shared_ptr<StderrLogger> create_stderr_logger(Level lv, Mod mod)
{
    return std::make_shared<StderrLogger>(lv, mod);
}

} // nemaspace log
