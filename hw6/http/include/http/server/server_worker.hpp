#ifndef HTTP_SERVER_WORKER_HPP
#define HTTP_SERVER_WORKER_HPP

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
#include <chrono>


namespace http
{


class ServerWorker
{
public:
    using EnqueueCallback = std::function<void(std::string_view)>;
    using Callback = std::function<network::HttpResponse(const network::HttpRequest&, EnqueueCallback&)>;
    using TimeoutType = std::chrono::duration<int, std::milli>;
    using TimePoint = std::chrono::time_point<std::chrono::system_clock, TimeoutType>;

    ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, TimeoutType read_timeout, TimeoutType write_timeout, Callback on_request);

    void run();

private:
    const utils::FileDescriptor& m_server_fd;
    utils::FileDescriptor m_epoll_fd;

    const bool& m_done;
    const TimeoutType m_write_timeout;
    const TimeoutType m_read_timeout;
    Callback m_on_request;

    std::unordered_map<int, utils::Coroutine::routine_t> m_clients;

private:
    void create_epoll();
    void add_epoll(int fd, uint32_t events) const;
    void accept_clients();
    void handle_client(int fd, epoll_event event);

    void check_clients_timeout();
    bool is_read_timed_out(TimePoint compared_time);
    bool is_write_timed_out(TimePoint compared_time);

    static TimePoint current_time();
};

} // namespace http

#endif // HTTP_SERVER_WORKER_HPP
