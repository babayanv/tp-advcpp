#ifndef HTTP_SERVER_WORKER_HPP
#define HTTP_SERVER_WORKER_HPP

#include "http/utils/file_descriptor.hpp"
#include "http/utils/coroutine.hpp"
#include "http/network/protocol/http_request.hpp"
#include "http/network/protocol/http_response.hpp"

#include <sys/epoll.h>

#include <string_view>
#include <unordered_set>
#include <functional>


namespace http
{


class ServerWorker
{
public:
    using Callback = std::function<network::HttpResponse(const network::HttpRequest&)>;

    ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, Callback on_request);

    void run();

private:
    const utils::FileDescriptor& m_server_fd;
    utils::FileDescriptor m_epoll_fd;

    const bool& m_done;

    Callback m_on_request;

    std::unordered_set<utils::Coroutine::routine_t> m_clients;

private:
    void create_epoll();
    void add_epoll(int fd, uint32_t events) const;
    void accept_clients();
    void handle_client(int fd, epoll_event event);
};

} // namespace http

#endif // HTTP_SERVER_WORKER_HPP
