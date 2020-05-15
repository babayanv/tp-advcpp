#include "http/network/protocol/http_response.hpp"
#include "http/utils/coroutine.hpp"

#include <sstream>


namespace http::network
{


bool send_response(Connection& conn, const HttpResponse& response)
{
    std::ostringstream oss;

    oss << response.version << ' ' << response.status << CRLF;

    for (auto& i : response.headers)
    {
        oss << i.first << ':' << i.second << CRLF;
    }

    oss << CRLF
        << response.body
        << CRLF << CRLF;

    std::string response_msg = oss.str();
    std::string_view response_msg_sv = response_msg;

    size_t bytes_written = conn.write(response_msg_sv.data(), static_cast<size_t>(oss.tellp()));
    while (bytes_written < static_cast<size_t>(oss.tellp()))
    {
        response_msg_sv.remove_prefix(bytes_written);

        utils::Coroutine::yield();

        bytes_written = conn.write(response_msg_sv.data(), static_cast<size_t>(oss.tellp()));
    }
}


}

