#ifndef HTTP_NETWORK_HTTP_RESPONSE_HPP
#define HTTP_NETWORK_HTTP_RESPONSE_HPP

#include "http/network/protocol/http.hpp"
#include "http/network/connection.hpp"

#include <map>


namespace http::network
{


struct HttpResponse
{
    std::string version;
    http::status::value_type status;
    std::map<std::string_view, std::string_view> headers;
    std::string body;
};


bool send_response(Connection& conn, const HttpResponse& response);


} // namespace http::network

#endif // HTTP_NETWORK_HTTP_RESPONSE_HPP
