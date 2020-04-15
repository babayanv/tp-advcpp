#ifndef SHMEM_EXCEPTION_HPP
#define SHMEM_EXCEPTION_HPP

#include <cstring>

#include <exception>
#include <string>
#include <sstream>


namespace shmem
{


class ErrnoException : public std::exception
{
public:
    ErrnoException()
        : m_msg(std::strerror(errno))
    {
    }

    explicit ErrnoException(const std::string& what_arg)
        : m_msg(what_arg)
    {
        m_msg += std::strerror(errno);
    }

    virtual ~ErrnoException() noexcept = default;

    const char* what() const noexcept override
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};


class SemaphoreError : public ErrnoException
{
public:
    SemaphoreError() = default;
    SemaphoreError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ShmemError : public ErrnoException
{
public:
    ShmemError() = default;
    ShmemError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ShmemInvalidArgument : public std::invalid_argument
{
public:
    ShmemInvalidArgument() = default;

    template <class... T>
    ShmemInvalidArgument(const std::string& what_arg, T&&... args)
        : invalid_argument(create_message(what_arg, std::forward<T>(args)...))
    {
    }

private:
    template <typename... T>
    std::string create_message(const std::string& what_arg, T&&... args)
    {
        std::stringstream ss;

        ss << "Invalid argument - ";
        [[maybe_unused]] int dummy[sizeof...(T)] = { (ss << args << ' ', 0)... };
        ss << ": " << what_arg;

        return ss.str();
    }

};

} // namespace shmem

#endif // SHMEM_EXCEPTION_HPP
