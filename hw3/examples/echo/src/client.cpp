#include "tcp/Connection.hpp"
#include <iostream>

int main()
{
    tcp::Connection conn("127.0.0.1", 8080);

    std::string msg(4, '\0');

    while (msg != "exit")
    {
        std::cin >> msg;

        conn.write(msg.size());
        conn.write(msg.c_str(), msg.size());

        size_t msg_size = 0;

        conn.read(msg_size);
        conn.read(msg.data(), msg_size);

        std::cout << msg_size << ": " << msg << std::endl;
    }

    return 0;
}