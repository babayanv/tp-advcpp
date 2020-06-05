#ifndef DB_SERVER_HPP
#define DB_SERVER_HPP

#include "http/server/server.hpp"
#include "data_bank/external_sort/data_file.hpp"


class DBServer : public http::Server
{
public:
    DBServer(std::string_view address, uint16_t port, size_t max_conn, data_bank::DataFile<uint64_t>& data);

    http::network::HttpResponse on_request(const http::network::HttpRequest& request) override;

private:
    data_bank::DataFile<uint64_t>& m_data;

    http::status::value_type m_status = http::status::S_200_OK;

private:
    bool validate_method(std::string_view method);
    bool validate_path(std::string_view path);
    bool validate_version(std::string_view version);
};

#endif // DB_SERVER_HPP