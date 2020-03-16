#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <iostream>
#include <bitset>


void send(proc::Process& p, const std::string& msg)
{
    if (msg.size() >= 256)
    {
        return;
    }

    char len_msg[3] = {0};
    std::snprintf(len_msg, 3, "%zu", msg.length());

    p.writeExact(len_msg, 3);
    p.writeExact(msg.c_str(), msg.length());
}


std::string receive(proc::Process& p)
{
    char buff[256] = {0};

    p.readExact(buff, 3);
    size_t length = std::stoul(std::string(buff, 3));

    p.readExact(buff, length);

    return std::string(buff, length);
}


int main()
{
    try
    {
        proc::Process proc("./echo_child");

        while (!feof(stdin))
        {
            std::string msg;
            std::cin >> msg;

            send(proc, msg);
            std::cout << receive(proc) << std::endl;
        }
    }
    catch (const proc::BadProcess& bp)
    {
        std::cerr << bp.what() << std::endl;

        return 1;
    }

    return 0;
}
