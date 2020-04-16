#include "epoll_server/Server.hpp"
#include "epoll_server/Exception.hpp"

#include <iostream>
#include <charconv>


constexpr size_t MB = 1048576;


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
                std::string msg(MB - conn.input_bytes(), '\0');

                try
                {
                    std::cout << "Received: " <<
                        conn.read(msg.data(), msg.size()) <<
                        " bytes" << std::endl;
                }
                catch(const es::ConnectionError& ce)
                {
                    std::cerr << ce.what() << std::endl;

                    std::string_view err_msg(ce.what());
                    std::cout << "Sent: " <<
                        conn.write(err_msg.data(), err_msg.size()) <<
                        " bytes" << std::endl;

                    return;
                }

                if (conn.input_bytes() == MB)
                {
                    std::string msg = conn.input_cat();
                    std::cout << "Sent: " <<
                        conn.write(msg.c_str(), msg.size()) <<
                        " bytes" << std::endl;
                    
                    conn.input_clear();
                }
            }
        };

    es::Server server(address, port, max_connect, handler);

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
