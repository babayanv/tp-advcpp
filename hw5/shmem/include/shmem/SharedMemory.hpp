#ifndef SHMEM_SHMEM_HPP
#define SHMEM_SHMEM_HPP

#include "shmem/Exception.hpp"
#include "shmem/utils/bridge/NonCopyable.hpp"

#include <sys/mman.h>
#include <unistd.h>

#include <memory>
#include <functional>
#include <iostream>
#include <iomanip>


namespace shmem
{


template <typename T>
using ShmemPtr = std::unique_ptr<T, std::function<void(T*)>>;


template <typename T>
ShmemPtr<T> make_shmem(size_t count = 1)
{
    void* mmap_ptr = ::mmap(nullptr, sizeof(T) * count,
                            PROT_WRITE | PROT_READ,
                            MAP_SHARED | MAP_ANONYMOUS,
                            -1, 0);
    if (mmap_ptr == MAP_FAILED)
    {
        throw ShmemError("mmap error: ");
    }

    return {
        static_cast<T*>(mmap_ptr),
        [count](T* t){
            ::munmap(t, sizeof(T) * count);
        }
    };
}


class SharedMemory : utils::bridge::NonCopyable
{
    using byte_type = std::byte;
    using boundary_ptr = byte_type*;

    struct boundaries {
        boundary_ptr m_begin;
        boundary_ptr m_end;
    };

public:
    SharedMemory(std::size_t page_count = 1)
    {
        auto page_size = static_cast<size_t>(::sysconf(_SC_PAGE_SIZE));

        m_shmem_ptr = make_shmem<byte_type>(page_count * page_size);
        m_boundaries = new (m_shmem_ptr.get()) boundaries{
            m_shmem_ptr.get() + sizeof(boundaries),
            m_shmem_ptr.get() + page_count * page_size
        };
    }


    SharedMemory(SharedMemory&& other)
        : m_shmem_ptr(std::move(other.m_shmem_ptr))
        , m_boundaries(std::exchange(other.m_boundaries, nullptr))
    {
    }


    ~SharedMemory() = default;


    SharedMemory& operator=(SharedMemory&& other)
    {
        m_shmem_ptr = std::move(other.m_shmem_ptr);
        m_boundaries = std::exchange(other.m_boundaries, nullptr);

        return *this;
    }


    template <class T = byte_type, class... Args>
    T* store(std::size_t count = 1, Args&&... args)
    {
        if (m_boundaries->m_begin + count * sizeof(T) >= m_boundaries->m_end)
        {
            throw std::bad_alloc();
        }

        boundary_ptr location = m_boundaries->m_begin;

        for (size_t i = 0; i < count; ++i)
        {
            new (m_boundaries->m_begin) T(std::forward<Args>(args)...);
            m_boundaries->m_begin += sizeof(T);
        }

        return reinterpret_cast<T*>(location);
    }


    template <class T>
    void free(T* dst, size_t count = 1)
    {
        if (reinterpret_cast<boundary_ptr>(dst) < m_shmem_ptr.get() ||
            reinterpret_cast<boundary_ptr>(dst) >= m_boundaries->m_begin)
        {
            throw ShmemInvalidArgument("dst is outside of reserved shared memory bounds", dst, m_boundaries->m_begin, m_boundaries->m_end);
        }

        if (reinterpret_cast<boundary_ptr>(dst + count) >= m_boundaries->m_end)
        {
            throw ShmemInvalidArgument("count makes dst block step outside shared memory bounds", count, m_boundaries->m_end);
        }

        T* end_of_destroyed_data = dst;
        for (size_t i = 0; i < count; ++i)
        {
            if (!std::is_trivially_destructible_v<T>)
            {
                end_of_destroyed_data->~T();
            }
            ++end_of_destroyed_data;
        }

        remove_data(dst, end_of_destroyed_data);
    }


    template <class T = byte_type>
    T* get(std::ptrdiff_t offset)
    {
        if (m_shmem_ptr.get() + offset >= m_boundaries->m_end)
        {
            throw ShmemInvalidArgument("Unable to get data - offset is too big", offset);
        }

        return reinterpret_cast<T*>(m_shmem_ptr.get() + offset);
    }


    template <class T, size_t Step = sizeof(T)>
    void dump(std::ostream& os)
    {
        os << std::setfill('-')
           << std::setw(60) << ""
           << std::setfill(' ') << std::endl

           << std::setw(10) << "type" << " | "
           << std::setw(8) << "byte" << " | "
           << std::setw(16) << "address" << " | "
           << std::setw(16) << "value" << std::endl

           << std::setfill('-')
           << std::setw(60) << ""
           << std::setfill(' ') << std::endl;

        for (auto i = m_shmem_ptr.get(); i < m_boundaries->m_end; i += Step)
        {
            T* ptr = reinterpret_cast<T*>(i);

            os << std::setw(10) << (i >= m_boundaries->m_begin ? "UNUSED" : "RESERVED") << " | "
               << std::setw(8) << i - m_shmem_ptr.get() << " | "
               << std::setw(16) << ptr << " | "
               << std::setw(16) << *ptr << std::endl;
        }

        os << std::setfill('-')
           << std::setw(60) << ""
           << std::setfill(' ') << std::endl;
    }

private:
    ShmemPtr<byte_type> m_shmem_ptr;
    boundaries* m_boundaries;

private:
    template <class T,
              typename std::enable_if_t<std::is_trivially_move_assignable_v<T>, int> = 0
    >
    void remove_data(T* begin, T* end)
    {
        std::memmove(reinterpret_cast<boundary_ptr>(begin),
                     reinterpret_cast<boundary_ptr>(end),
                     m_boundaries->m_end - reinterpret_cast<boundary_ptr>(end));

        m_boundaries->m_begin = reinterpret_cast<boundary_ptr>(end);
    }

    template <class T,
              typename std::enable_if_t<!std::is_trivially_move_assignable_v<T>, int> = 0
    >
    void remove_data(T* begin, T* end)
    {
        ptrdiff_t count = end - begin;

        while (reinterpret_cast<boundary_ptr>(end) < m_boundaries->m_end)
        {
            *begin = std::move(*end);
            ++begin;
            ++end;
        }

        m_boundaries->m_begin -= count * sizeof(T);
    }
};

} // namespace shmem

#endif // SHMEM_SHMEM_HPP
