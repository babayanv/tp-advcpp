#ifndef SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP
#define SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP


namespace shmem::utils::bridge
{

class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable& other) = delete;

    NonCopyable& operator=(const NonCopyable& other) = delete;

    virtual ~NonCopyable() noexcept = default;

};

} // namespace shmem

#endif // SHMEM_UTILS_BRIDGES_NONCOPYABLE_HPP
