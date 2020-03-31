#ifndef SERVER_HPP
#define SERVER_HPP

#include "tcp/Connection.hpp"


namespace tcp
{


class Server
{
public:
    Server(const std::string& address, uint16_t port, int max_connect = 0);
    ~Server() noexcept;

    void open(const std::string& address, uint16_t port);
    void close();

    Connection accept();

    void set_max_connect(int max_connect);
    bool is_opened();

private:
    int m_sock_fd;
    bool m_opened;

};

} // namespace tcp

#endif // SERVER_HPP
