#include "shmem/container/Map.hpp"

#include <unistd.h>
#include <sys/wait.h>

#include <string>


int main()
{
    shmem::Map<std::string, std::string, 2> map;

    int pid = fork();

    if (pid != 0)
    {
        map.emplace("0", "zero");
        map.emplace("2", "two");
        map.emplace("4", "four");
        map.emplace("6", "six");
        map.emplace("8", "eight");
    }
    else
    {
        map.emplace("1", "one");
        map.emplace("3", "three");
        map.emplace("5", "five");
        map.emplace("7", "seven");
        map.emplace("9", "nine");

        return 0;
    }

    waitpid(pid, nullptr, 0);

    for (auto i : map)
    {
        std::cout << i.first << ' ' << i.second << std::endl;
    }

    return 0;
}
