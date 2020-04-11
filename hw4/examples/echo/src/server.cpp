#include "epoll_server/Server.hpp"
#include "epoll_server/Exception.hpp"

#include <iostream>


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port> <max_connection_count>" << std::endl;
        return 1;
    }

    auto handler =
        [](es::Connection& conn){
            if (conn.events().size() == 1)
            {
                std::cout << "Connected: " << conn.addr() << ':' << conn.port() << std::endl;
            }

            uint32_t recent_events = conn.recent_event().events;

            if (recent_events & EPOLLHUP ||
                recent_events & EPOLLERR ||
                recent_events & EPOLLRDHUP)
            {
                std::cout << "Disconnected: " << conn.addr() << ':' << conn.port() << std::endl;
                return;
            }

            if (conn.recent_event().events & EPOLLIN)
            {
                std::string msg(128, '\0');
                if (conn.read(msg.data(), msg.size()) == 0)
                {
                    return;
                }

                std::cout << "Received: " << msg << std::endl;

                conn.setReadyToWrite();
            }

            if (conn.recent_event().events & EPOLLOUT)
            {
                const std::string msg("pong");
                if (conn.write(msg.c_str(), msg.size()) == 0)
                {
                    return;
                }

                std::cout << "Sent: " << msg << std::endl;

                conn.setReadyToRead();
            }
        };

    es::Server server(argv[1], strtoul(argv[2], NULL, 10), atoi(argv[3]), handler);;

    while (true)
    {
        try
        {
            server.run();
        }
        catch(const es::ServerError& se)
        {
            std::cerr << se.what() << std::endl;

            server.init(argv[1], strtoul(argv[2], NULL, 10), atoi(argv[3]), handler);
            continue;
        }
    }

    return 0;
}
