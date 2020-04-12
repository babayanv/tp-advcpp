#include "tcp/Server.hpp"
#include "tcp/Exception.hpp"

#include <iostream>
#include <cstring>


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port> <max_connection_count>" << std::endl;
        return 1;
    }

    tcp::Server server(argv[1], strtoul(argv[2], NULL, 10), atoi(argv[3]));

    tcp::Connection conn(server.accept());

    std::cout << "Connection accepted: " <<
        conn.get_addr() << ':' << conn.get_port() <<
        std::endl;

    conn.set_receive_timeout(5);

    std::string msg;
    size_t time_out_counter = 0;

    while (conn.is_opened())
    {
        size_t msg_size = 0;

        try
        {
            conn.read(msg_size);
            msg.resize(msg_size);

            conn.readExact(msg.data(), msg_size);
        }
        catch(const tcp::SocketError& se)
        {
            std::cerr << se.what() << std::endl;

            if (time_out_counter >= 5)
            {
                throw se;
            }

            ++time_out_counter;
            continue;
        }

        conn.write(msg_size);
        conn.writeExact(msg.c_str(), msg_size);

        std::cout << msg_size << ": " << msg << std::endl;
    }

    return 0;
}
