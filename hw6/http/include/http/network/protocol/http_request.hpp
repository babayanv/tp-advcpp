#ifndef HTTP_NETWORK_HTTP_REQUEST_HPP
#define HTTP_NETWORK_HTTP_REQUEST_HPP

#include "http/network/protocol/http.hpp"

#include <map>


namespace http::network
{


class HttpRequest
{
public:
    method::value_type method{};
    std::string path{};
    std::map<std::string, std::string> query_params{};
    std::string_view version{};
    std::map<std::string_view, std::string_view> headers{};
    std::string_view body{};

    explicit HttpRequest(std::string_view request_sv);

private:
    std::string_view m_request_sv{};

private:
    void parse_method();
    void parse_path();
    void parse_query_string(std::string_view query_string);
    void parse_version();
    void parse_headers();
    void parse_body();
};


std::string unescape(std::string_view origin);


} // namespace server

#endif // HTTP_NETWORK_HTTP_REQUEST_HPP
