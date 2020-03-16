#include <unistd.h>
#include <cstdio>
#include <string>
#include <bitset>


void send(FILE* stream, const std::string& msg)
{
    if (msg.length() >= 256)
    {
        return;
    }

    char len_msg[3] = {0};
    std::snprintf(len_msg, 3, "%zu", msg.length());

    fwrite(len_msg, 1, 3, stream);
    fwrite(msg.c_str(), 1, msg.length(), stream);

    fflush(stream);
}


std::string receive(FILE* stream)
{
    char buff[256] = {0};

    fread(buff, 1, 3, stream);

    size_t length = std::stoul(std::string(buff, 3));

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
