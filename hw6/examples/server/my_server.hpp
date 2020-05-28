#ifndef MY_SERVER_HPP
#define MY_SERVER_HPP

#include "http/server/server.hpp"
#include "log/Logger.hpp"


class MyServer : public http::Server
{
public:
    MyServer(std::string_view address, uint16_t port, size_t max_conn)
        : Server(address, port, max_conn)
    {
    }


    http::network::HttpResponse on_request(const http::network::HttpRequest& request)
    {
        log::info("MY LOG MSG - received request:");
        validate_method(request.method) && validate_path(request.path) && validate_version(request.version);

        http::network::HttpResponse response{};
        response.version = request.version;
        response.status = m_status;
        response.headers = request.headers;
        response.body = request.body;

        return response;
    }

private:
    http::status::value_type m_status = http::status::S_200_OK;

    bool validate_method(std::string_view method)
    {
        if (method != http::method::M_GET &&
            method != http::method::M_POST)
        {
            m_status = http::status::S_405_MNA;
            return false;
        }

        return true;
    }


    bool validate_path(std::string_view path)
    {
        if (path.find("../") != std::string::npos)
        {
            m_status = http::status::S_403_F;
            return false;
        }

        return true;
    }


    bool validate_version(std::string_view version)
    {
        if (!version.starts_with("HTTP/"))
        {
            m_status = http::status::S_400_BR;
            return false;
        }

        return true;
    }
};

#endif // MY_SERVER_HPP