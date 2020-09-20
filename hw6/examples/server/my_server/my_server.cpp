#include "my_server.hpp"

#include <filesystem>


MyServer::MyServer(std::string_view address, uint16_t port, size_t max_conn, std::string_view doc_root)
    : Server(address, port, max_conn)
    , m_doc_root(doc_root)
{
}


http::network::HttpResponse MyServer::on_request(const http::network::HttpRequest& request)
{
    // Can use log:: here
    // Ex. - log::info("Request received!");

    validate_method(request.method) && validate_path(request.path) && validate_version(request.version);

    http::network::HttpResponse response{};
    response.version = request.version;
    response.status = m_status;
    response.headers.emplace("Connection", "Keep-Alive");

    std::string file_path = m_doc_root + request.path;
    if (is_file_available(file_path, request.method))
    {
        response.files.emplace(file_path); // entered file will be sent after the returned response
    }

    return response;
}


bool MyServer::validate_method(std::string_view method)
{
    if (method != http::method::M_GET &&
        method != http::method::M_POST)
    {
        m_status = http::status::S_405_MNA;
        return false;
    }

    return true;
}


bool MyServer::validate_path(std::string_view path)
{
    if (path.find("../") != std::string::npos)
    {
        m_status = http::status::S_403_F;
        return false;
    }

    return true;
}


bool MyServer::validate_version(std::string_view version)
{
    if (!version.starts_with("HTTP/"))
    {
        m_status = http::status::S_400_BR;
        return false;
    }

    return true;
}


bool MyServer::is_file_available(std::string_view file_path, http::method::value_type method) {
    namespace fs = std::filesystem;

    if (m_status == http::status::S_200_OK && fs::is_directory(file_path))
    {
        m_status = http::status::S_403_F;
    }

    if (m_status == http::status::S_200_OK && !fs::exists(file_path))
    {
        m_status = http::status::S_404_NF;
    }

    return m_status == http::status::S_200_OK && method == http::method::M_GET;
}
