#include "utils/parser.hpp"
#include "my_server.hpp"

#include "http/errors.hpp"
#include "http/signal.hpp"
#include "log/Logger.hpp"
#include "log/loggers/StderrLogger.hpp"

#include <iostream>


int main(int argc, char* argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " <ipv4_address> <port> <max_connection_count> <thread_limit> <document_root>" << std::endl;
        return 1;
    }

    auto [ address, port, max_conn, thread_limit, doc_root ] = utils::parse_args(argv);

    try
    {
        log::init(log::create_stderr_logger(log::Level::DEBUG));

        MyServer server(address, port, max_conn);

        http::Signal::register_handler(SIGINT, &MyServer::handle_signal, &server);
        http::Signal::register_handler(SIGTERM, &MyServer::handle_signal, &server);

        MyServer::TimeoutType read_timeout{10};
        MyServer::TimeoutType write_timeout{10};

        server.run(thread_limit, read_timeout, write_timeout);
    }
    catch (const http::ServerError& se)
    {
        std::cerr << se.what() << std::endl;
    }

    return 0;
}