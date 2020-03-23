#ifndef BASE_FORMATTER_HPP
#define BASE_FORMATTER_HPP

#include "log/Properties.hpp"

#include <string>


namespace log
{


class BaseFormatter
{
public:
    BaseFormatter(Mod mod) : m_mod(mod) {}
    virtual ~BaseFormatter() = default;

    inline void set_mod(Mod mod) noexcept
    {
        m_mod = mod;
    }

    inline Mod mod() noexcept
    {
        return m_mod;
    }

    inline bool is_mod(Mod mod) noexcept
    {
        return m_mod & mod;
    }

    virtual std::string fmt(const std::string& msg, Level lv) noexcept = 0;

private:
    Mod m_mod;
};

} // namespace log

#endif // BASE_FORMATTER_HPP
