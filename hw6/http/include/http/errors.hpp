#ifndef HTTP_ERRORS_HPP
#define HTTP_ERRORS_HPP

#include <exception>
#include <string>


namespace http
{


class ErrnoException : public std::exception
{
public:
    ErrnoException();
    explicit ErrnoException(std::string what_arg);

    ~ErrnoException() noexcept override = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};


class FDError : public ErrnoException
{
public:
    FDError() = default;
    explicit FDError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ConnectionError : public ErrnoException
{
public:
    ConnectionError() = default;
    explicit ConnectionError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ServerError : public ErrnoException
{
public:
    ServerError() = default;
    explicit ServerError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class TimeOut : public std::exception
{
};


} // namespace http

#endif // HTTP_ERRORS_HPP
