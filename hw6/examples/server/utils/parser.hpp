#ifndef PARSER_HPP
#define PARSER_HPP

#include <tuple>


namespace utils
{

std::tuple<std::string_view, uint16_t, size_t, size_t, std::string_view> parse_args(char* argv[]);

} // namespace utils

#endif // PARSER_HPP
