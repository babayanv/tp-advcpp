#ifndef THREADED_LOGGER_HPP
#define THREADED_LOGGER_HPP

#include "log/Logger.hpp"
#include "log/utils/SafeQueue.hpp"

#include <string>
#include <thread>
#include <functional>


namespace log
{


using BaseLoggerPtr = std::shared_ptr<BaseLogger>;

class ThreadedLogger
{
    using CallbackType = std::function<void(const std::string&)>;
    using QueueDataType = std::tuple<std::string, CallbackType>;

public:
    static ThreadedLogger& get_instance();

    void enqueue_log(const std::string& msg, CallbackType cb);

    void stop() noexcept;

private:
    utils::SafeQueue<QueueDataType> m_queue;

    bool m_done;
    std::thread m_worker;

private:
    ThreadedLogger();
    ~ThreadedLogger() noexcept;
    ThreadedLogger(const ThreadedLogger& other) = delete;
    ThreadedLogger& operator=(const ThreadedLogger& other) = delete;

    void work();
    void log(const QueueDataType& data);
};

void debug_tl(const std::string& log_msg);
void info_tl(const std::string& log_msg);
void warn_tl(const std::string& log_msg);
void error_tl(const std::string& log_msg);

} // namespace log

#endif // THREADED_LOGGER_HPP
