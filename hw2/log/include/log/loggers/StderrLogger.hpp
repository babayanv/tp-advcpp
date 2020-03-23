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

    virtual inline void flush() noexcept override
    {
        std::cerr.flush();
    }

private:
    virtual inline void log_impl(const std::string& log_msg) noexcept override
    {
        std::cerr << log_msg << std::endl;
    }

};


inline std::shared_ptr<StderrLogger> create_stderr_logger(Level lv, Mod mod = Mod::ALL)
{
    return std::make_shared<StderrLogger>(lv, mod);
}


} // namespace log

#endif // STDERR_LOGGER_HPP