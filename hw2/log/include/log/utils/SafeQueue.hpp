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
    using DataPtr = std::shared_ptr<DataType>;

public:
    SafeQueue() = default;
    ~SafeQueue() = default;

    bool enqueue(DataPtr elem)
    {
        if (elem == nullptr)
        {
            return false;
        }

        std::lock_guard<std::mutex> lock(m_mut);
        m_queue.push(elem);

        return true;
    }

    DataPtr dequeue()
    {
        if (m_queue.empty())
        {
            return nullptr;
        }

        DataPtr elem = m_queue.front();

        std::lock_guard<std::mutex> lock(m_mut);
        m_queue.pop();

        return elem;
    }

    inline bool empty()
    {
        return m_queue.empty();
    }

private:
    std::queue<DataPtr> m_queue;
    std::mutex m_mut;

};

} // namespace utils

} // namespace log

#endif // SAFE_QUEUE_HPP