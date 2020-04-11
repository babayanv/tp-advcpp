#include "epoll_server/Server.hpp"
#include "epoll_server/Exception.hpp"

#include <iostream>
#include <charconv>


auto parse_args(char* argv[])
{
    std::string_view address(argv[1]);

    uint16_t port{};
    {
        std::string_view sv(argv[2]);
        std::from_chars(sv.data(), sv.data() + sv.size(), port);
    }

    int max_connect{};
    {
        std::string_view sv(argv[3]);
        std::from_chars(sv.data(), sv.data() + sv.size(), max_connect);
    }

    return std::make_tuple<std::string>(argv[1], port, max_connect);
}


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port> <max_connection_count>" << std::endl;
        return 1;
    }

    auto&& [ address, port, max_connect ] = parse_args(argv);

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

    es::Server server(address, port, max_connect, handler);;

    try
    {
        server.run();
    }
    catch(const es::ServerError& se)
    {
        std::cerr << se.what() << std::endl;
    }

    return 0;
}
