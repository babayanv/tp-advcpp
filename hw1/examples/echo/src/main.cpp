#include "process/Process.hpp"
#include "process/Exception.hpp"

#include <iostream>


void send(proc::Process& p, const std::string& msg)
{
    size_t msg_length = msg.size();

    p.write(msg_length);
    p.writeExact(msg.c_str(), msg.size());
}


std::string receive(proc::Process& p)
{
    size_t msg_length = 0;
    p.read(msg_length);

    std::string received_msg(msg_length, '\0');

    p.readExact(received_msg.data(), received_msg.size());

    return received_msg;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << std::string{argv[0]} << " <path_to_executable>" << std::endl;
        return 1;
    }

    try
    {
        proc::Process proc(argv[1]);

        while (!feof(stdin))
        {
            std::string msg{};
            std::cin >> msg;

            send(proc, msg);
            std::cout << receive(proc) << std::endl;
        }
    }
    catch (const proc::ProcessError& bp)
    {
        std::cerr << bp.what() << std::endl;
        return 1;
    }

    return 0;
}
