#ifndef HTTP_SIGNAL_HPP
#define HTTP_SIGNAL_HPP

#include <functional>


namespace http
{


class Signal
{
public:
    using Handler = std::function<void()>;

    static void register_handler(int signal, const Handler& handler);

    template <class Func, class ...Args, class = std::enable_if_t<!std::is_invocable_v<Func>>>
    static void register_handler(int signal, Func&& func, Args&&... args)
    {
        register_handler(signal, std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    }

private:
    static std::unordered_multimap<int, Handler> s_sig2handler;

private:
    static void handle(int signal);
};


} // namespace http

#endif // HTTP_SIGNAL_HPP
