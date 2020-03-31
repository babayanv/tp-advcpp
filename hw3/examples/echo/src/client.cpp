#include "tcp/Connection.hpp"
#include <iostream>


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port>" << std::endl;
        return 1;
    }

    tcp::Connection conn(argv[1], atoi(argv[2]));

    std::cout << "Connected to: " <<
        conn.get_addr() << ':' << conn.get_port() <<
        std::endl;

    while (!std::cin.eof())
    {
        std::string msg;
        std::cin >> msg;

        conn.write(msg.size());
        conn.writeExact(msg.c_str(), msg.size());

        size_t msg_size = 0;

        conn.read(msg_size);
        conn.readExact(msg.data(), msg_size);

        std::cout << msg_size << ": " << msg << std::endl;
    }

    return 0;
}
