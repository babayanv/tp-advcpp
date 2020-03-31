#include "tcp/Server.hpp"
#include "tcp/Exception.hpp"

#include <iostream>
#include <cstring>


int main()
{
    tcp::Server server("127.0.0.1", 8080, 1);

    tcp::Connection conn(std::move(server.accept()));

    std::cout << "Connection accepted\n";

    conn.set_receive_timeout(5);
    conn.set_send_timeout(5);


    while (std::cin.eof())
    {
        std::string msg(4, '\0');

        size_t msg_size = 0;

        try
        {
            conn.read(msg_size);
            msg.resize(msg_size);
            conn.readExact(msg.data(), msg_size);
        }
        catch(const tcp::SocketError& se)
        {
            std::cout << "read timed out" << std::endl;
            continue;
        }

        try
        {
            conn.write(msg_size);
            conn.writeExact(msg.c_str(), msg_size);
        }
        catch(const tcp::SocketError& se)
        {
            std::cout << "write timed out" << std::endl;
            continue;
        }

        std::cout << msg_size << ": " << msg << std::endl;
    }

    return 0;
}
