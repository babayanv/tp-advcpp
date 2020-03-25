#include "log/ThreadedLogger.hpp"
#include "log/loggers/StdoutLogger.hpp"
#include "log/loggers/FileLogger.hpp"


int main()
{
    log::init(log::create_stdout_logger(log::Level::DEBUG));

    log::debug_tl("debug message");
    log::info_tl("info message");
    log::warn_tl("warning message");
    log::error_tl("error message");

    return 0;
}
