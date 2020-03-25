#ifndef SAFE_QUEUE_HPP
#define SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <memory>


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

    template <class ... Args>
    void enqueue(Args&&... args)
    {
        std::lock_guard lock(m_mut);
        m_queue.emplace(std::forward<Args>(args)...);
    }

    DataType dequeue()
    {
        DataType& elem = m_queue.front();

        std::lock_guard lock(m_mut);
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

};

} // namespace utils

} // namespace log

#endif // SAFE_QUEUE_HPP
