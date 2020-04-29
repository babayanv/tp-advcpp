#ifndef SHMEM_UTILS_SEMAPHORE_HPP
#define SHMEM_UTILS_SEMAPHORE_HPP

#include "shmem/Exception.hpp"
#include "shmem/utils/bridge/NonCopyable.hpp"

#include <semaphore.h>

#include <iostream>


namespace shmem::utils
{

class Semaphore : bridge::NonCopyable
{
public:
    explicit Semaphore(uint accessors_count = 1)
        : m_semaphore{}
    {
        init(accessors_count);
    }


    ~Semaphore() override = default;


    void init(uint accessors_count = 0)
    {
        if (::sem_init(&m_semaphore, 1, accessors_count) < 0)
        {
            throw SemaphoreError("Semaphore init error: ");
        }
    }


    void destroy()
    {
        if (::sem_destroy(&m_semaphore) < 0)
        {
            throw SemaphoreError("Semaphore destroy error: ");
        }
    }


    void acquire()
    {
        while (::sem_wait(&m_semaphore) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            throw SemaphoreError("Semaphore acquire error: ");
        }
    }


    void release(size_t update = 1)
    {
        for (size_t i = 0; i < update; ++i)
        {
            if (::sem_post(&m_semaphore) < 0)
            {
                throw SemaphoreError("Semaphore release error: ");
            }
        }
    }

private:
    sem_t m_semaphore;
};


class SemaphoreLock
{
public:
    explicit SemaphoreLock(Semaphore& sem)
        : m_sem(sem)
    {
        m_sem.acquire();
    }

    ~SemaphoreLock()
    {
        m_sem.release();
    }

private:
    Semaphore& m_sem;
};

} // namespace shmem

#endif // SHMEM_UTILS_SEMAPHORE_HPP
