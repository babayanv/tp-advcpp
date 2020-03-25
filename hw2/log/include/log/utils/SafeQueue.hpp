#ifndef SAFE_QUEUE_HPP
#define SAFE_QUEUE_HPP

#include <queue>
#include <condition_variable>
#include <mutex>

namespace log
{

namespace utils
{

template <class DataType>
class SafeQueue
{
public:
    SafeQueue() = default;
    ~SafeQueue() = default;


    template <class... Args>
    void enqueue(Args&&... args)
    {
        {
            std::lock_guard lock(m_mut);
            m_queue.emplace(std::forward<Args>(args)...);
        }

        m_cv.notify_one();
    }


    DataType dequeue()
    {
        std::unique_lock lock(m_mut);

        while (empty())
        {
            m_cv.wait(lock);
        }

        auto elem = std::move(m_queue.front());
        m_queue.pop();

        return elem;
    }


    bool empty()
    {
        return m_queue.empty();
    }

private:
    std::queue<DataType> m_queue;

    std::mutex m_mut;
    std::condition_variable m_cv;
};

} // namespace utils

} // namespace log

#endif // SAFE_QUEUE_HPP
