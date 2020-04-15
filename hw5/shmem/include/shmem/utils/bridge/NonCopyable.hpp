#ifndef SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP
#define SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP


namespace shmem
{

namespace utils
{

namespace bridge
{

class NonCopyable
{
public:
    NonCopyable() = default;
    virtual ~NonCopyable() noexcept = default;

private:
    NonCopyable(const NonCopyable& other) = delete;
    NonCopyable& operator=(const NonCopyable& other) = delete;
};

} // namespace bridge

} // namespace utils

} // namespace shmem

#endif // SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP
