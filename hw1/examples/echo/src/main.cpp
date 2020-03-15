#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <iostream>
#include <bitset>


void send(proc::Process& p, const std::string& msg)
{
    if (msg.length() >= 256)
    {
        return;
    }

    std::bitset<8> bs(msg.length());

    p.write(bs.to_string().c_str(), 8);
    p.write(msg.c_str(), msg.length());
}


std::string receive(proc::Process& p)
{
    char buff[256] = {0};

    p.read(buff, 8);

    std::bitset<8> bs(buff);
    size_t length = bs.to_ulong();

    p.read(buff, length);

    return std::string(buff, length);
}


int main()
{
    proc::Process proc("./echo_child");

    while (!feof(stdin))
    {
        std::string msg;
        std::cin >> msg;

        send(proc, msg);
        std::cout << receive(proc) << std::endl;
    }

    return 0;
}
