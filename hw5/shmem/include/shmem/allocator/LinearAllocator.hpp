#ifndef SHMEM_ALLOCATOR_LINEAR_ALLOCATOR_HPP
#define SHMEM_ALLOCATOR_LINEAR_ALLOCATOR_HPP

#include "shmem/SharedMemory.hpp"

namespace shmem::allocator
{

template <class T>
class LinearAllocator
{
public:
    using value_type = T;

public:
    constexpr explicit LinearAllocator(SharedMemory* shmem)
        : m_shmem(shmem)
    {
    }


    template <class U>
    constexpr explicit LinearAllocator(const LinearAllocator<U>& other)
        : m_shmem(other.m_shmem)
    {
    }

    ~LinearAllocator() = default;

    value_type* allocate(std::size_t count)
    {
        return m_shmem->allocate<value_type>(count);
    }

    void deallocate(value_type* ptr, std::size_t count)
    {
        m_shmem->deallocate(ptr, count);
    }

public:
    SharedMemory* m_shmem;
};

} // namespace shmem

#endif // SHMEM_ALLOCATOR_LINEAR_ALLOCATOR_HPP
