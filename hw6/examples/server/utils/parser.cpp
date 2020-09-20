#include "parser.hpp"

#include <charconv>


namespace utils
{


std::tuple<std::string_view, uint16_t, size_t, size_t, std::string_view> parse_args(char* argv[])
{
    uint16_t port{};
    {
        std::string_view sv(argv[2]);
        std::from_chars(sv.data(), sv.data() + sv.size(), port);
    }

    size_t max_conn{};
    {
        std::string_view sv(argv[3]);
        std::from_chars(sv.data(), sv.data() + sv.size(), max_conn);
    }

    size_t thread_limit{};
    {
        std::string_view sv(argv[4]);
        std::from_chars(sv.data(), sv.data() + sv.size(), thread_limit);
    }

    return std::make_tuple(argv[1], port, max_conn, thread_limit, argv[5]);
}


}
