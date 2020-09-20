#include "http/network/protocol/http_response.hpp"
#include "http/utils/coroutine.hpp"

#include <sstream>


namespace http::network
{


std::string HttpResponse::to_string() const
{
    std::ostringstream oss;

    oss << version << ' ' << status << CRLF;

    for (auto& [ header, value ] : headers)
    {
        oss << header << ':' << value << CRLF;
    }

    if (!body.empty())
    {
        oss << CRLF << body << CRLF;
    }

    if (files.empty())
    {
        oss << CRLF;
    }

    return oss.str();
}


}

