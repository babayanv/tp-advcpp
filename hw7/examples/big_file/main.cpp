#include "file_generator.hpp"
#include "db_server.hpp"

#include "data_bank/external_sort/data_file.hpp"
#include "http/errors.hpp"
#include "http/signal.hpp"


int main()
{
    constexpr std::string_view file_name = "data_collection";

    constexpr size_t generated_file_size = 160;
    auto [ key, value, actual_file_size ] = get_file<uint64_t, uint64_t>(file_name, generated_file_size);

    constexpr size_t partition_size = 32;
    data_bank::DataFile<decltype(value)> data(file_name, partition_size, "CacheFile.txt");

    try
    {
        DBServer server("127.0.0.1", 8080, 1024, data);

        http::Signal::register_handler(SIGINT, &DBServer::handle_signal, &server);
        http::Signal::register_handler(SIGTERM, &DBServer::handle_signal, &server);
        ::signal(SIGPIPE, SIG_IGN);

        DBServer::TimeoutType read_timeout{5000};
        DBServer::TimeoutType write_timeout{5000};

        server.run(4, read_timeout, write_timeout);
    }
    catch (const http::ServerError& se)
    {
        std::cerr << se.what() << std::endl;
    }

    return 0;
}