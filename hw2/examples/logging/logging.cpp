#include "log/Logger.hpp"
#include "log/loggers/StdoutLogger.hpp"
#include "log/loggers/FileLogger.hpp"
#include "log/loggers/StderrLogger.hpp"


int main()
{
    log::init(log::create_stdout_logger(log::Level::ALL));

    log::debug("debug message");
    log::info("info message");
    log::warn("warning message");
    log::error("error message");

    log::init(log::create_file_logger("logger.log", log::Level::ALL));

    log::debug("debug message");
    log::info("info message");
    log::warn("warning message");
    log::error("error message");

    log::init(log::create_stderr_logger(log::Level::ALL));

    log::debug("debug message");
    log::info("info message");
    log::warn("warning message");
    log::error("error message");

    return 0;
}
