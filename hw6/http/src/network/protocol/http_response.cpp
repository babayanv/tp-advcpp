#include "http/network/protocol/http_response.hpp"
#include "http/utils/coroutine.hpp"

#include <sstream>


namespace http::network
{


std::string HttpResponse::to_string()
{
    std::ostringstream oss;

    oss << version << ' ' << status << CRLF;

    for (auto& i : headers)
    {
        oss << i.first << ':' << i.second << CRLF;
    }

    oss << CRLF
        << body
        << CRLF << CRLF;

    return oss.str();
}


}

