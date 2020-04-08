#include "epoll_server/Server.hpp"

#include <iostream>


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port> <max_connection_count>" << std::endl;
        return 1;
    }

    es::Server server(argv[1], strtoul(argv[2], NULL, 10),
        [](es::Connection& conn){
            std::string msg(4, '\0');
            conn.readExact(msg.data(), msg.size());

            std::cout << "Received: " << msg << std::endl;
        },
        [](es::Connection& conn){
            std::string msg("pong");

            std::cout << "Sent: " << msg << std::endl;
            conn.writeExact(msg.data(), msg.size());
        },
        atoi(argv[3]));

    server.run();

    return 0;
}