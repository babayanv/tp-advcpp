#include "log/ThreadedLogger.hpp"


namespace log
{


ThreadedLogger& ThreadedLogger::get_instance()
{
    static ThreadedLogger s_instance;
    return s_instance;
}


void ThreadedLogger::enqueue_log(const std::string& msg, CallbackType cb) noexcept
{
    std::unique_lock<std::mutex> lock(m_mut);

    QueueDataType data{msg, cb};
    m_queue.enqueue(std::make_shared<QueueDataType>(data));

    m_notified = true;
    m_cv.notify_one();
}


void ThreadedLogger::stop()
{
    m_done = true;
    m_cv.notify_one();

    if (m_worker.joinable())
    {
        m_worker.join();
    }
}


ThreadedLogger::ThreadedLogger()
    : m_done(false)
    , m_worker([this] { this->work(); })
{
}


ThreadedLogger::~ThreadedLogger() noexcept
{
    stop();
}


void ThreadedLogger::work()
{
    std::unique_lock<std::mutex> lock(m_mut);

    while (!m_done)
    {
        while (!m_notified)
        {
            m_cv.wait(lock);
        }

        while (!m_queue.empty())
        {
            log(*m_queue.dequeue()); 
        }

        m_notified = false;
    }

    while (!m_queue.empty())
    {
        log(*m_queue.dequeue()); 
    }
}

} // namespcae log
