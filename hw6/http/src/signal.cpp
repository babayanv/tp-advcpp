#include "http/signal.hpp"

#include <csignal>


namespace http
{


void Signal::register_handler(int signal, const Handler& handler)
{
    std::signal(signal, handle);

    s_sig2handler.emplace(signal, handler);
}


void Signal::handle(int signal)
{
    auto [ begin, end ] = std::equal_range(s_sig2handler.begin(), s_sig2handler.end(), signal);
    for (auto& elem = begin; elem != end; ++elem)
    {
        elem->second();
    }
}


} // namespace http