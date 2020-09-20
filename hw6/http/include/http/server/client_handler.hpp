#ifndef HTTP_CLIENT_HANDLER_HPP
#define HTTP_CLIENT_HANDLER_HPP

#include "http/network/connection.hpp"
#include "http/utils/coroutine.hpp"
#include "http/network/protocol/http_request.hpp"
#include "http/network/protocol/http_response.hpp"

#include <chrono>


namespace http
{


struct HandlerContext;


class ClientHandler
{
public:
    using Callback = std::function<network::HttpResponse(const network::HttpRequest&)>;
    using TimeoutType = std::chrono::duration<int, std::milli>;
    using TimePoint = std::chrono::time_point<std::chrono::system_clock, TimeoutType>;

public:
    ClientHandler(int fd, const HandlerContext& ctx);

    void handle();

private:
    const HandlerContext& m_ctx;

    network::Connection m_conn;
    TimePoint m_last_resumed;

private:
    bool try_send_response(network::HttpResponse& response);

    bool is_timed_out();
    bool is_read_timed_out();
    bool is_write_timed_out();

    static TimePoint current_time();
};


struct HandlerContext
{
    ClientHandler::TimeoutType read_timeout;
    ClientHandler::TimeoutType write_timeout;
    ClientHandler::Callback on_request;
};


} // namespace http

#endif // HTTP_CLIENT_HANDLER_HPP
