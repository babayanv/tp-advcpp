#ifndef HTTP_NETWORK_HTTP_RESPONSE_HPP
#define HTTP_NETWORK_HTTP_RESPONSE_HPP

#include "http/network/protocol/http.hpp"

#include <unordered_map>
#include <queue>


namespace http::network
{


struct HttpResponse
{
    std::string version{};
    http::status::value_type status{};
    std::unordered_map<std::string_view, std::string_view> headers{};
    std::string body{};
    std::queue<std::string> files;

    std::string to_string() const;
};


} // namespace http::network

#endif // HTTP_NETWORK_HTTP_RESPONSE_HPP
