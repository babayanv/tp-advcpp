#include "log/StdoutLogger.hpp"


namespace log
{


std::shared_ptr<StdoutLogger> create_stdout_logger(Level lv)
{
    auto logger = std::make_shared<StdoutLogger>();
    logger->set_level(lv);

    return logger;
}

} // namespace log
