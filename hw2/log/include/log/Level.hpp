#ifndef LEVEL_HPP
#define LEVEL_HPP


namespace log
{

enum class Level
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    ALL,
};


namespace color
{
    // redundant const specifier is to get rid of compiler warning since ISO C++ forbids converting a string constant to ‘char*’
    constexpr const char* DEFAULT = "\033[0m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* WHITE = "\033[37m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* RED = "\033[31m";

} // namespace color


namespace font
{
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* BOLD_OFF = "\033[22m";

} // namespace font

} // namespace log

#endif // LEVEL_HPP
