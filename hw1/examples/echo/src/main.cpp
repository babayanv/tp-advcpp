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

    size_t msg_length = msg.size();

    p.writeExact(&msg_length, sizeof(size_t));
    p.writeExact(msg.c_str(), msg.size());
}


std::string receive(proc::Process& p)
{
    size_t msg_length = 0;
    p.readExact(&msg_length, sizeof(size_t));

    char buff[256] = {0};
    p.readExact(buff, msg_length);

    return std::string{buff, msg_length};
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
