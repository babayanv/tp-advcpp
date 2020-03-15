#include <unistd.h>
#include <cstdio>
#include <string>
#include <bitset>


void send(FILE* stream, std::string& msg)
{
    if (msg.length() >= 256)
    {
        return;
    }

    std::bitset<8> bs(msg.length());

    fwrite(bs.to_string().c_str(), 1, 8, stream);
    fwrite(msg.c_str(), 1, msg.length(), stream);

    fflush(stream);
}


std::string receive(FILE* stream)
{
    char buff[256] = {0};

    fread(buff, 1, 8, stream);

    std::string buff_str(buff, 8);
    size_t length = std::stoul(buff_str, nullptr, 2);

    fread(buff, 1, length, stream);

    return std::string(buff, length);
}


int main()
{
    while (!feof(stdin))
    {
        std::string msg = receive(stdin);
        send(stdout, msg);
    }

    return 0;
}
