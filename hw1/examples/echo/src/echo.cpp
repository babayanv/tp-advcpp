#include <unistd.h>
#include <cstdio>
#include <string>


void send(FILE* stream, const std::string& msg)
{
    size_t msg_length = msg.size();

    fwrite(&msg_length, sizeof(size_t), 1, stream);
    fwrite(msg.c_str(), 1, msg_length, stream);
    fflush(stream);
}


std::string receive(FILE* stream)
{
    size_t msg_length = 0;
    fread(&msg_length, sizeof(size_t), 1, stream);

    std::string received_msg(msg_length, '\0');

    fread(received_msg.data(), 1, received_msg.size(), stream);

    return received_msg;
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
