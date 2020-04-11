#ifndef SERVER_HPP
#define SERVER_HPP

#include "epoll_server/utils/FileDescriptor.hpp"
#include "epoll_server/Connection.hpp"

#include <map>
#include <functional>


namespace es
{


class Server
{
    using Callback = std::function<void(Connection&)>;

public:
    Server() = default;

    Server(const std::string& address, uint16_t port,
           int max_connect = 0,
           Callback&& do_handle_client = [](Connection&){});

    Server(uint16_t port,
           int max_connect = 0,
           Callback&& do_handle_client = [](Connection&){});

    ~Server() noexcept = default;

    Server(const Server& other) = delete;
    Server(Server&& other) = delete;
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other) = delete;

    void init(const std::string& address, uint16_t port,
              int max_connect = 0,
              Callback&& do_handle_client = [](Connection&){});
    void init(uint16_t port,
              int max_connect = 0,
              Callback&& do_handle_client = [](Connection&){});
    void open(const std::string& address, uint16_t port);
    void open(uint16_t port);
    void listen(int max_connect);
    void close();

    bool is_opened();

    void run();

private:
    utils::FileDescriptor m_sock_fd;
    utils::FileDescriptor m_epoll_fd;
    std::map<int, Connection> m_connections;

    Callback m_do_handle_client;

private:
    void create_epoll();
    void add_epoll(int fd, uint32_t events);
    void accept_clients();
    void handle_client(int fd, epoll_event event);
};

} // namespace es

#endif // SERVER_HPP
