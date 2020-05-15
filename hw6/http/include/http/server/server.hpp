#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "http/utils/file_descriptor.hpp"
#include "http/network/protocol/http_request.hpp"
#include "http/network/protocol/http_response.hpp"
#include "http/server/server_worker.hpp"
#include "http/errors.hpp"

#include <arpa/inet.h>

#include <csignal>
#include <thread>
#include <vector>


namespace http
{

template <class Derived>
class Server
{
    using ThreadPool = std::vector<std::thread>;

public:
    Server(const Server& other) = delete;
    Server(Server&& other) = delete;
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other) = delete;


    static Server& init(std::string_view address, uint16_t port, size_t max_conn)
    {
        static Server s_instance(address, port, max_conn);
        return s_instance;
    }


    static Server& get_instance()
    {
        return init("", 0, 0);
    }


    void open(std::string_view address, uint16_t port)
    {
        if (is_opened())
        {
            close();
        }

        m_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (m_fd < 0)
        {
            throw ServerError("Error creating socket: ");
        }

        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = ::htons(port);
        if (::inet_aton(address.data(), &serv_addr.sin_addr) == 0)
        {
            throw ServerError("Invalid ipv4 address: ");
        }

        if (::bind(m_fd,
                   reinterpret_cast<sockaddr*>(&serv_addr),
                   sizeof(serv_addr)) != 0)
        {
            throw ServerError("Error binding socket: ");
        }
    }


    void listen(int max_connect) const
    {
        if (::listen(m_fd, max_connect) != 0)
        {
            throw ServerError("Socket listen failed: ");
        }
    }


    void close()
    {
        m_fd.close();
    }


    bool is_opened() const noexcept
    {
        return m_fd.is_opened();
    }


    void run(size_t thread_limit)
    {
        m_done = false;

        m_threads.reserve(thread_limit);

        for (size_t i = 0; i < thread_limit - 1; ++i)
        {
            m_threads.emplace_back(
                    [this]
                    {
                        ServerWorker sw(m_fd, m_done, [this](const network::HttpRequest& request) { return static_cast<Derived*>(this)->on_request(request); } );
                        sw.run();
                    });
        }

        ServerWorker sw(m_fd, m_done, [this](const network::HttpRequest& request) { return static_cast<Derived*>(this)->on_request(request); });
        sw.run();
    }


    void join_threads()
    {
        if (m_done)
        {
            return;
        }

        m_done = true;

        for (auto& i : m_threads)
        {
            i.join();
        }
    }

private:
    utils::FileDescriptor m_fd;

    ThreadPool m_threads;
    bool m_done = false;

private:
    Server(std::string_view address, uint16_t port, size_t max_conn)
    {
        std::signal(SIGINT, handle_signal);
        std::signal(SIGTERM, handle_signal);

        try
        {
            open(address, port);
            listen(static_cast<int>(max_conn));
        }
        catch (const ServerError& se)
        {
            close();
            throw;
        }
    }


    ~Server() noexcept
    {
        join_threads();
    }


    static void handle_signal(int)
    {
        get_instance().join_threads();
        get_instance().close();
    }
};

} // namespace http

#endif // HTTP_SERVER_HPP
