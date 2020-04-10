#include "epoll_server/Connection.hpp"

#include <iostream>
#include <chrono>
#include <thread>


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port>" << std::endl;
        return 1;
    }

    es::Connection conn(argv[1], strtoul(argv[2], NULL, 10));

    std::cout << "Connected to: " <<
        conn.addr() << ':' << conn.port() <<
        std::endl;

    while (!std::cin.eof())
    {
        std::string msg("ping");

        conn.write_exact(msg.c_str(), msg.size());
        std::cout << "Sent: " << msg << std::endl;

        while(conn.read(msg.data(), 4) == 0);
        std::cout << "Received: " << msg << std::endl;

        std::chrono::seconds dur(5);
        std::this_thread::sleep_for(dur);
    }

    return 0;
}