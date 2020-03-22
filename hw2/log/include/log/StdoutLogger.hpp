#ifndef STDOUT_LOGGER_HPP
#define STDOUT_LOGGER_HPP

#include "log/BaseLogger.hpp"

#include <iostream>
#include <memory>


namespace log
{

class StdoutLogger final : public BaseLogger
{
public:
    StdoutLogger() = default;

    StdoutLogger(const StdoutLogger& other) = delete;
    StdoutLogger& operator=(const StdoutLogger& other) = delete;

    StdoutLogger(StdoutLogger&& other) = delete;
    StdoutLogger& operator=(StdoutLogger&& other) = delete;

    virtual inline void flush() noexcept override
    {
        std::cout.flush();
    }

private:
    virtual inline void log_impl(const std::string& log_msg) noexcept override
    {
        std::cout << log_msg;
    }

};


std::shared_ptr<StdoutLogger> create_stdout_logger(Level lv);

} // namespace log

#endif // STDOUT_LOGGER_HPP