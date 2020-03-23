#ifndef THREADED_LOGGER_HPP
#define THREADED_LOGGER_HPP

#include "log/Logger.hpp"
#include "log/utils/SafeQueue.hpp"

#include <string>
#include <thread>
#include <condition_variable>
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

    void enqueue_log(const std::string& msg, CallbackType cb) noexcept;

    inline void log(const QueueDataType& data)
    {
        auto [ str, callback ] = data;
        callback(str);
    }

    void stop();

private:
    std::mutex m_mut;
    std::condition_variable m_cv;
    utils::SafeQueue<QueueDataType> m_queue;
    bool m_done;
    bool m_notified;

    std::thread m_worker;

private:
    ThreadedLogger();
    ~ThreadedLogger() noexcept;
    ThreadedLogger(const ThreadedLogger& other) = delete;
    ThreadedLogger& operator=(const ThreadedLogger& other) = delete;

    void work();
};

inline void debug_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, debug);
}
inline void info_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, info);
}
inline void warn_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, warn);
}
inline void error_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, error);
}

} // namespace log

#endif // THREADED_LOGGER_HPP