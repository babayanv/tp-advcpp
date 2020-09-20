#ifndef MY_SERVER_HPP
#define MY_SERVER_HPP

#include "http/server/server.hpp"

#include "log/Logger.hpp"


class MyServer : public http::Server
{
public:
    MyServer(std::string_view address, uint16_t port, size_t max_conn, std::string_view doc_root);

    http::network::HttpResponse on_request(const http::network::HttpRequest& request) override;

private:
    std::string m_doc_root;
    http::status::value_type m_status = http::status::S_200_OK;

private:
    bool validate_method(std::string_view method);
    bool validate_path(std::string_view path);
    bool validate_version(std::string_view version);

    bool is_file_available(std::string_view file_path, http::method::value_type method);
};

#endif // MY_SERVER_HPP
