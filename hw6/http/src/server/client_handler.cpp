#include "http/server/client_handler.hpp"


namespace http
{


ClientHandler::ClientHandler(int fd, const HandlerContext& ctx)
    : m_ctx(ctx)
    , m_conn(fd)
    , m_last_resumed(current_time())
{
}


void ClientHandler::handle()
{
    while (true)
    {
        std::string msg_received = m_conn.read_all(4096);
        if (msg_received.empty())
        {
            utils::Coroutine::yield();

            if (is_timed_out())
            {
                return;
            }

            continue;
        }

        network::HttpRequest request(msg_received);
        network::HttpResponse response = m_ctx.on_request(request);

        if (!try_send_response(response))
        {
            return;
        }

        auto elem = request.headers.find("Connection");
        if (elem == request.headers.end() || elem->second != "Keep-Alive")
        {
            return;
        }
    }
}


bool ClientHandler::try_send_response(network::HttpResponse& response)
{
    std::string msg_to_send = response.to_string();
    std::string_view msg_to_send_sv = msg_to_send;

    size_t bytes_written = m_conn.write(msg_to_send_sv.data(), msg_to_send_sv.size());
    while (bytes_written < msg_to_send_sv.size())
    {
        msg_to_send_sv.remove_prefix(bytes_written);

        utils::Coroutine::yield();
        if (is_timed_out())
        {
            return false;
        }

        bytes_written = m_conn.write(msg_to_send_sv.data(), msg_to_send_sv.size());
    }

    while (!response.files.empty())
    {
        m_conn.send_file(response.files.front());
        response.files.pop();
    }

    return true;
}


bool ClientHandler::is_timed_out()
{
    if (is_read_timed_out() || is_write_timed_out())
    {
        return true;
    }

    m_last_resumed = current_time();

    return false;
}


bool ClientHandler::is_read_timed_out()
{
    return (current_time() - m_last_resumed) >= m_ctx.read_timeout;
}


bool ClientHandler::is_write_timed_out()
{
    return (current_time() - m_last_resumed) >= m_ctx.write_timeout;
}


ClientHandler::TimePoint ClientHandler::current_time()
{
    return std::chrono::time_point_cast<TimeoutType>(std::chrono::system_clock::now());
}


} // namespace http
