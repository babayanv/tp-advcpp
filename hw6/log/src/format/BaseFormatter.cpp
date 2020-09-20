#include "log/format/BaseFormatter.hpp"


namespace log
{


BaseFormatter::BaseFormatter(Mod mod)
    : m_mod(mod)
{
}


void BaseFormatter::set_mod(Mod mod) noexcept
{
    m_mod = mod;
}


Mod BaseFormatter::mod() noexcept
{
    return m_mod;
}


bool BaseFormatter::is_mod(Mod mod) noexcept
{
    return m_mod & mod;
}

} // namespace log