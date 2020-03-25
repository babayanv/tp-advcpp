#include "log/ThreadedLogger.hpp"


namespace log
{


ThreadedLogger::ThreadedLogger()
    : m_done(false)
    , m_worker([this] { this->work(); })
{
}


ThreadedLogger::~ThreadedLogger() noexcept
{
    stop();
}


ThreadedLogger& ThreadedLogger::get_instance()
{
    static ThreadedLogger s_instance;
    return s_instance;
}


void ThreadedLogger::enqueue_log(const std::string& msg, CallbackType cb) noexcept
{
    if (m_done)
    {
        return;
    }

    m_queue.enqueue(msg, cb);
}


void ThreadedLogger::stop()
{
    m_done = true;

    if (m_worker.joinable())
    {
        m_worker.join();
    }
}


void ThreadedLogger::work()
{
    while (!m_done)
    {
        log(m_queue.dequeue()); 
    }

    while (!m_queue.empty())
    {
        log(m_queue.dequeue()); 
    }
}


void ThreadedLogger::log(const QueueDataType& data)
{
    auto& [ str, callback ] = data;
    callback(str);
}


void debug_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, debug);
}


void info_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, info);
}


void warn_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, warn);
}


void error_tl(const std::string& log_msg)
{
    ThreadedLogger::get_instance().enqueue_log(log_msg, error);
}

} // namespcae log
