#ifndef HTTP_NETWORK_HTTP_REQUEST_HPP
#define HTTP_NETWORK_HTTP_REQUEST_HPP

#include "http/network/protocol/http.hpp"

#include <map>


namespace http::network
{


struct HttpRequest
{
    method::value_type method;
    std::string path;
    std::map<std::string, std::string> query_params;
    std::string_view version;
    std::map<std::string_view, std::string_view> headers;
    std::string_view body;
};


HttpRequest build_request(std::string_view request_sv);

std::string unescape(std::string_view origin);


class RequestBuilder
{
public:
    RequestBuilder() = default;

    HttpRequest operator()(std::string_view request_sv);

private:
    std::string_view m_request_sv;
    HttpRequest m_request{};

private:
    void get_method();
    void get_path();
    void get_version();
    void get_headers();
    void get_body();

    void parse_query_string(std::string_view query_string);
};


} // namespace server

#endif // HTTP_NETWORK_HTTP_REQUEST_HPP
