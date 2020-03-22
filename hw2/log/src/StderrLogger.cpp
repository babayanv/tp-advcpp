#include "log/StderrLogger.hpp"


namespace log
{


std::shared_ptr<StderrLogger> create_stderr_logger(Level lv)
{
    auto logger = std::make_shared<StderrLogger>();
    logger->set_level(lv);

    return logger;
}

} // nemaspace log
