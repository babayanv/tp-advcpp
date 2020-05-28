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
    auto [ begin, end ] = s_sig2handler.equal_range(signal);
    for (auto& elem = begin; elem != end; ++elem)
    {
        elem->second();
    }
}


} // namespace http