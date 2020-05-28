#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "http/utils/file_descriptor.hpp"
#include "http/network/protocol/http_request.hpp"
#include "http/network/protocol/http_response.hpp"

#include <thread>
#include <vector>


namespace http
{


class Server
{
    using ThreadPool = std::vector<std::thread>;

public:
    Server(std::string_view address, uint16_t port, size_t max_conn);
    virtual ~Server() noexcept;

    Server(const Server& other) = delete;
    Server(Server&& other) = delete;
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other) = delete;

    void open(std::string_view address, uint16_t port);
    void listen(int max_connect) const;
    void close();

    bool is_opened() const noexcept;

    void run(size_t thread_limit);

    void join_threads();

    virtual void handle_signal();

    virtual network::HttpResponse on_request(const network::HttpRequest& request) = 0;

private:
    utils::FileDescriptor m_fd;

    ThreadPool m_threads;
    bool m_done = false;
};

} // namespace http

#endif // HTTP_SERVER_HPP
