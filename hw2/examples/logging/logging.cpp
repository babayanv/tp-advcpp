#include "log/Logger.hpp"
#include "log/StdoutLogger.hpp"
#include "log/FileLogger.hpp"


int main()
{
    log::init(log::create_stdout_logger(log::Level::ERROR));

    log::debug("debug message");
    log::info("info message");
    log::warn("warning message");
    log::error("error message");

    return 0;
}
