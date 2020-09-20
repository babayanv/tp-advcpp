#ifndef BASE_FORMATTER_HPP
#define BASE_FORMATTER_HPP

#include "log/Properties.hpp"

#include <string>


namespace log
{


class BaseFormatter
{
public:
    BaseFormatter(Mod mod);
    virtual ~BaseFormatter() = default;

    void set_mod(Mod mod) noexcept;
    Mod mod() noexcept;
    bool is_mod(Mod mod) noexcept;

    virtual std::string fmt(const std::string& msg, Level lv) noexcept = 0;

private:
    Mod m_mod;
};

} // namespace log

#endif // BASE_FORMATTER_HPP
