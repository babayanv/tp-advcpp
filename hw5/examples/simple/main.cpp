#include "shmem/container/Map.hpp"
#include "shmem/Type.hpp"


#include <unistd.h>
#include <sys/wait.h>


int main()
{
    shmem::SharedMemory shmem;
    shmem::Map<shmem::String, shmem::String> map(shmem);

    int pid = fork();

    if (pid != 0)
    {
        shmem::StringAllocator alloc(shmem);
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple("0", alloc),
                    std::forward_as_tuple("zero", alloc));
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple("2", alloc),
                    std::forward_as_tuple("two", alloc));
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple("4", alloc),
                    std::forward_as_tuple("four", alloc));
    }
    else
    {
        shmem::StringAllocator alloc(shmem);
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple("1", alloc),
                    std::forward_as_tuple("one", alloc));
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple("3", alloc),
                    std::forward_as_tuple("three", alloc));
        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple(1024, 'a', alloc),
                    std::forward_as_tuple(1024, 'b', alloc));

        return 0;
    }

    waitpid(pid, nullptr, 0);

    for (const auto& i : map)
    {
        std::cout << i.first << ' ' << i.second << std::endl;
    }

    return 0;
}
