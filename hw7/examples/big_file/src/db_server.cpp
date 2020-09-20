#include "db_server.hpp"

#include <filesystem>
#include <charconv>


DBServer::DBServer(std::string_view address, uint16_t port, size_t max_conn, data_bank::DataFile<uint64_t>& data)
    : Server(address, port, max_conn)
    , m_data(data)
{
}


http::network::HttpResponse DBServer::on_request(const http::network::HttpRequest& request)
{
    validate_method(request.method) && validate_path(request.path) && validate_version(request.version);

    http::network::HttpResponse response{};
    response.version = request.version;
    response.status = m_status;
    response.headers.emplace("Connection", "Keep-Alive");

    std::string_view path_sv(request.path);

    size_t pos = path_sv.find_last_of('/');
    path_sv.remove_prefix(pos + 1);

    uint64_t key{};
    std::from_chars(path_sv.begin(), path_sv.end(), key);

    auto [ value, success ] = m_data.at(key);

    if (!success)
    {
        response.status = http::status::S_404_NF;
    }
    else
    {
        response.status = http::status::S_200_OK;
        response.body += std::to_string(value);
    }

    return response;
}



bool DBServer::validate_method(std::string_view method)
{
    if (method != http::method::M_GET)
    {
        m_status = http::status::S_405_MNA;
        return false;
    }

    return true;
}


bool DBServer::validate_path(std::string_view path)
{
    if (path.find("../") != std::string::npos)
    {
        m_status = http::status::S_403_F;
        return false;
    }

    return true;
}


bool DBServer::validate_version(std::string_view version)
{
    if (!version.starts_with("HTTP/"))
    {
        m_status = http::status::S_400_BR;
        return false;
    }

    return true;
}
