#ifndef LOG_EXCEPTION_HPP
#define LOG_EXCEPTION_HPP

#include <exception>
#include <string>


namespace log
{

class LoggerError : public std::exception
{
public:
    explicit LoggerError(const std::string& what_arg);
    explicit LoggerError(const char* what_arg);

    virtual ~LoggerError() noexcept = default;

    virtual const char* what() const noexcept;

private:
    std::string m_msg;
};


class LoggerThreadStopped : public LoggerError
{
public:
    explicit LoggerThreadStopped(const std::string& what_arg);
    explicit LoggerThreadStopped(const char* what_arg);

    ~LoggerThreadStopped() noexcept = default;
};

} // namespace log

#endif // LOG_EXCEPTION_HPP