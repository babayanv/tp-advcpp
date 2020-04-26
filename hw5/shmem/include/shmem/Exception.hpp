#ifndef SHMEM_EXCEPTION_HPP
#define SHMEM_EXCEPTION_HPP

#include <cstring>

#include <exception>
#include <string>
#include <sstream>
#include <utility>


namespace shmem
{


class ErrnoException : public std::exception
{
public:
    ErrnoException()
        : m_msg(std::strerror(errno))
    {
    }

    explicit ErrnoException(std::string  what_arg)
        : m_msg(std::move(what_arg))
    {
        m_msg += std::strerror(errno);
    }

    ~ErrnoException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override
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
    explicit SemaphoreError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ShmemError : public ErrnoException
{
public:
    ShmemError() = default;
    explicit ShmemError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class ShmemInvalidArgument : public std::invalid_argument
{
public:
    template <class... T>
    explicit ShmemInvalidArgument(const std::string& what_arg, T&&... args)
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
