#ifndef TCP_EXCEPTION_HPP
#define TCP_EXCEPTION_HPP

#include <exception>
#include <string>


namespace tcp
{

class SocketError : public std::exception
{
public:
    SocketError();
    explicit SocketError(const std::string& what_arg);

    virtual ~SocketError() noexcept = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};

} // namespace tcp

#endif // TCP_EXCEPTION_HPP
