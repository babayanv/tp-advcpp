#ifndef HTTP_NETWORK_HTTP_RESPONSE_HPP
#define HTTP_NETWORK_HTTP_RESPONSE_HPP

#include "http/network/protocol/http.hpp"

#include <map>


namespace http::network
{


struct HttpResponse
{
    std::string version{};
    http::status::value_type status{};
    std::map<std::string_view, std::string_view> headers{};
    std::string body{};

    std::string to_string();
};


} // namespace http::network

#endif // HTTP_NETWORK_HTTP_RESPONSE_HPP
