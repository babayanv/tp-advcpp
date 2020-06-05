#ifndef HTTP_SERVER_WORKER_HPP
#define HTTP_SERVER_WORKER_HPP

#include "http/server/client_handler.hpp"
#include "http/utils/file_descriptor.hpp"
#include "http/utils/coroutine.hpp"
#include "http/network/protocol/http_request.hpp"
#include "http/network/protocol/http_response.hpp"
#include "http/network/connection.hpp"

#include <sys/epoll.h>

#include <string_view>
#include <unordered_map>
#include <queue>
#include <functional>


namespace http
{


class ServerWorker
{
public:
    using Callback = ClientHandler::Callback;
    using TimeoutType = ClientHandler::TimeoutType;

    ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, HandlerContext ctx);

    void run();

private:
    const utils::FileDescriptor& m_server_fd;
    utils::FileDescriptor m_epoll_fd;

    const bool& m_done;
    const HandlerContext& m_ctx;
    std::unordered_map<int, utils::Coroutine::routine_t> m_clients;

private:
    void create_epoll();
    void add_epoll(int fd, uint32_t events) const;
    void accept_clients();
    void handle_client(int fd, epoll_event event);

    void check_clients_timeout();
};

} // namespace http

#endif // HTTP_SERVER_WORKER_HPP
