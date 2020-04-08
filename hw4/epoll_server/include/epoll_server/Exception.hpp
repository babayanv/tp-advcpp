#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>


namespace es
{

class ErrnoException : public std::exception
{
public:
    ErrnoException();
    explicit ErrnoException(const std::string& what_arg);

    virtual ~ErrnoException() noexcept = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};


class FDError : public ErrnoException
{
public:
    FDError() = default;
    FDError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ConnectionError : public ErrnoException
{
public:
    ConnectionError() = default;
    ConnectionError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ServerError : public ErrnoException
{
public:
    ServerError() = default;
    ServerError(const std::string& what_arg) : ErrnoException(what_arg) {}
};

} // namespace es

#endif // EXCEPTION_HPP