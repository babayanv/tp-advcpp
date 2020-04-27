#ifndef SHMEM_CONTAINER_MAP_HPP
#define SHMEM_CONTAINER_MAP_HPP

#include "shmem/allocator/LinearAllocator.hpp"
#include "shmem/utils/sync/Semaphore.hpp"
#include "shmem/Type.hpp"

#include <map>

#include <tuple>
#include <utility>


namespace shmem
{


template <class Key, class T, class Compare = std::less<Key>>
class Map
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using allocator_type = allocator::LinearAllocator<value_type>;

    using semaphore_type = utils::Semaphore;
    using internal_map_type = std::map<key_type,
                                       mapped_type,
                                       Compare,
                                       allocator_type>;
    using iterator = typename internal_map_type::iterator;
    using const_iterator = typename internal_map_type::const_iterator;

public:
    explicit Map(SharedMemory& shmem)
        : m_shmem(shmem)
    {
        m_semaphore_ptr = m_shmem.allocate<semaphore_type>();
        new (m_semaphore_ptr) utils::Semaphore();

        allocator_type alloc(m_shmem);

        m_map_ptr = m_shmem.allocate<internal_map_type>();
        new (m_map_ptr) internal_map_type(std::move(alloc));
    }

    ~Map() = default;


// Element access
    auto& at(const key_type& key)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->at(key);
    }

    const auto& at(const key_type& key) const
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->at(key);
    }

    auto& operator[](const key_type& key)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->operator[](key);
    }

    auto& operator[](key_type&& key)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->operator[](std::move(key));
    }


// Iterators
    auto begin() noexcept
    {
        return m_map_ptr->begin();
    }

    auto cbegin() const noexcept
    {
        return m_map_ptr->cbegin();
    }

    auto end() noexcept
    {
        return m_map_ptr->end();
    }

    auto cend() const noexcept
    {
        return m_map_ptr->cend();
    }


// Modifiers
    template <class... Args>
    auto emplace(Args&&... args)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->emplace(std::forward<Args>(args)...);
    }

    auto insert(const value_type& value)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->insert(value);
    }

    auto erase(iterator pos)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->erase(pos);
    }

    auto erase(const_iterator first, const_iterator last)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->erase(first, last);
    }

    auto erase(const key_type& key)
    {
        utils::SemaphoreLock lock(*m_semaphore_ptr);
        return m_map_ptr->erase(key);
    }

protected:
    SharedMemory& m_shmem;
    semaphore_type* m_semaphore_ptr;
    internal_map_type* m_map_ptr;
};

} // namespace shmem

#endif // SHMEM_CONTAINER_MAP_HPP
