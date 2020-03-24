#ifndef STDERR_LOGGER_HPP
#define STDERR_LOGGER_HPP

#include "log/loggers/BaseLogger.hpp"

#include <iostream>
#include <memory>


namespace log
{

class StderrLogger final : public BaseLogger
{
public:
    explicit StderrLogger(Level lv, Mod mod);

    StderrLogger(const StderrLogger& other) = delete;
    StderrLogger& operator=(const StderrLogger& other) = delete;

    StderrLogger(StderrLogger&& other) = delete;
    StderrLogger& operator=(StderrLogger&& other) = delete;

    virtual void flush() noexcept override;

private:
    virtual void log_impl(const std::string& log_msg) noexcept override;

};


std::shared_ptr<StderrLogger> create_stderr_logger(Level lv, Mod mod = Mod::ALL);


} // namespace log

#endif // STDERR_LOGGER_HPP
