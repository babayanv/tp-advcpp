#include "http/server/server_worker.hpp"
#include "http/errors.hpp"

#include <arpa/inet.h>

#include <utility>
#include <sstream>


namespace http
{


ServerWorker::ServerWorker(const utils::FileDescriptor& server_fd, const bool& done, TimeoutType read_timeout, TimeoutType write_timeout, Callback on_request)
    : m_server_fd(server_fd)
    , m_done(done)
    , m_write_timeout(write_timeout)
    , m_read_timeout(read_timeout)
    , m_on_request(std::move(on_request))
{
    create_epoll();
    add_epoll(m_server_fd, EPOLLIN | EPOLLEXCLUSIVE);
}


void ServerWorker::run()
{
    constexpr size_t EPOLL_SIZE = 128;
    epoll_event events[EPOLL_SIZE];

    int epoll_timeout = std::min(m_write_timeout.count(), m_read_timeout.count());

    while (!m_done)
    {
        int fd_count = epoll_wait(m_epoll_fd, events, EPOLL_SIZE, epoll_timeout);
        if (fd_count < 0)
        {
            if (errno == EINTR)
            {
                check_clients_timeout();
                continue;
            }

            throw ServerError("Error waiting epoll: ");
        }

        for (int i = 0; i < fd_count; ++i)
        {
            int fd = events[i].data.fd;

            if (fd == m_server_fd)
            {
                accept_clients();
            }
            else
            {
                auto client = m_clients.find(fd);

                if (client == m_clients.end())
                {
                    m_clients.emplace(fd, utils::Coroutine::create_and_run(&ServerWorker::handle_client, this, fd, events[i]));
                }
                else
                {
                    utils::Coroutine::resume(client->second);
                }
            }
        }
    }
}


void ServerWorker::create_epoll()
{
    m_epoll_fd = epoll_create(1);
    if (m_epoll_fd < 0)
    {
        throw ServerError("Error creating epoll: ");
    }
}


void ServerWorker::add_epoll(int fd, uint32_t events) const
{
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        throw ServerError("Error adding fd to epoll: ");
    }
}


void ServerWorker::accept_clients()
{
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t addr_size = sizeof(client_addr);

        int conn_fd = ::accept4(m_server_fd,
                                reinterpret_cast<sockaddr*>(&client_addr),
                                &addr_size,
                                SOCK_NONBLOCK);
        if (conn_fd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }

            throw ServerError("Error accepting connection: ");
        }

        add_epoll(conn_fd, EPOLLIN | EPOLLEXCLUSIVE);
    }
}


void ServerWorker::handle_client(int fd, epoll_event event)
{
    if (event.events != EPOLLIN)
    {
        return;
    }

    network::Connection conn(fd);
    TimePoint last_resumed = current_time();

    while (true)
    {
        std::string msg_received = conn.read_all(4096);

        if (msg_received.empty())
        {
            if (is_read_timed_out(last_resumed))
            {
                return;
            }

            utils::Coroutine::yield();
            last_resumed = current_time();
            continue;
        }

        network::HttpRequest request(msg_received);

        std::string msg_to_send = m_on_request(request).to_string();
        std::string_view msg_to_send_sv = msg_to_send;
        size_t bytes_written = conn.write(msg_to_send_sv.data(), msg_to_send_sv.size());

        while (bytes_written < msg_to_send_sv.size())
        {
            msg_to_send_sv.remove_prefix(bytes_written);

            utils::Coroutine::yield();

            if (is_write_timed_out(last_resumed))
            {
                return;
            }

            bytes_written = conn.write(msg_to_send_sv.data(), msg_to_send_sv.size());
        }

        auto elem = request.headers.find("Connection");
        if (elem == request.headers.end() || elem->second != "Keep-Alive")
        {
            break;
        }
    }
}


void ServerWorker::check_clients_timeout()
{
    for (auto& client : m_clients)
    {
        utils::Coroutine::resume(client.second);
    }
}


bool ServerWorker::is_read_timed_out(TimePoint compared_time)
{
    return (current_time() - compared_time) >= m_read_timeout;
}


bool ServerWorker::is_write_timed_out(TimePoint compared_time)
{
    return (current_time() - compared_time) >= m_write_timeout;
}


ServerWorker::TimePoint ServerWorker::current_time()
{
    return std::chrono::time_point_cast<TimeoutType>(std::chrono::system_clock::now());
}


} // namespace http
