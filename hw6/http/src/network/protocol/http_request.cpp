#include "http/network/protocol/http_request.hpp"


namespace http::network
{


HttpRequest build_request(std::string_view request_sv)
{
    RequestBuilder rb;
    return rb(request_sv);
}


std::string url_decode(std::string_view sv)
{
    std::string decoded_str(sv);

    for (size_t url_encode_pos = sv.find('%'); url_encode_pos != std::string::npos; url_encode_pos = sv.find('%'))
    {
        std::string_view url_encode = sv.substr(url_encode_pos + 1, 2);

        char url_decode = static_cast<char>(std::stoul(url_encode.data(), nullptr, 16));
        if (url_decode == 0)
        {
            continue;
        }

        decoded_str.replace(url_encode_pos, 3, 1, url_decode);
    }

    return decoded_str;
}


HttpRequest RequestBuilder::operator()(std::string_view request_sv)
{
    m_request_sv = request_sv;

    get_method();
    get_path();
    get_version();
    get_headers();
    get_body();

    return m_request;
}


void RequestBuilder::get_method()
{
    size_t pos = m_request_sv.find(" /");

    m_request.method = m_request_sv.substr(0, pos);
    m_request_sv.remove_prefix(pos + 1);
}


void RequestBuilder::get_path()
{
    size_t pos = m_request_sv.find(" HTTP/");

    m_request.path = m_request_sv.substr(0, pos);

    size_t query_string_pos = m_request.path.find('?');
    if (query_string_pos != std::string::npos)
    {
        parse_query_string(m_request.path.substr(query_string_pos));
        m_request.path = m_request.path.substr(0, query_string_pos);
    }

    m_request_sv.remove_prefix(pos + 1);
}


void RequestBuilder::get_version()
{
    size_t pos = m_request_sv.find(CRLF);

    m_request.version = m_request_sv.substr(0, pos);
    m_request_sv.remove_prefix(pos + CRLF.size());
}


void RequestBuilder::get_headers()
{
    while (m_request_sv.find(CRLF) != std::string_view::npos && !m_request_sv.starts_with(CRLF))
    {
        size_t header_name_pos = m_request_sv.find(':');
        if (header_name_pos == std::string_view::npos)
        {
            break;
        }
        std::string_view header_name = m_request_sv.substr(0, header_name_pos);
        m_request_sv.remove_prefix(header_name_pos + 1);

        size_t whitespaces_pos = m_request_sv.find_first_not_of(' ');
        m_request_sv.remove_prefix(whitespaces_pos);

        size_t header_value_pos = m_request_sv.find(CRLF);
        std::string_view header_value = m_request_sv.substr(0, header_value_pos);
        m_request_sv.remove_prefix(header_value_pos + CRLF.size());

        m_request.headers.emplace(header_name, header_value);
    }
}


void RequestBuilder::get_body()
{
    size_t pos = m_request_sv.find(CRLF);

    m_request.body = m_request_sv.substr(0, pos);
    m_request_sv.remove_prefix(pos + CRLF.size());
}


void RequestBuilder::parse_query_string(std::string_view query_string)
{
    while (true)
    {
        size_t query_param_name_pos = query_string.find('=');
        if (query_param_name_pos == std::string_view::npos)
        {
            break;
        }
        std::string_view query_param_name = query_string.substr(0, query_param_name_pos);
        query_string.remove_prefix(query_param_name_pos + 1);

        size_t query_param_value_pos = query_string.find('&');
        if (query_param_value_pos == std::string_view::npos)
        {
            break;
        }
        std::string_view query_param_value = query_string.substr(0, query_param_value_pos);
        query_string.remove_prefix(query_param_value_pos + 1);

        m_request.query_params.emplace(query_param_name, query_param_value);
    }
}


} // namespace server