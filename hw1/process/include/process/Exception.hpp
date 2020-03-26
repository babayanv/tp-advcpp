#ifndef PROC_EXCEPTION_HPP
#define PROC_EXCEPTION_HPP

#include <exception>
#include <cstring>


namespace proc
{

struct ProcessError : public std::exception {
    const char* what() const noexcept override {
        return std::strerror(errno);
    }
};

} // namespace proc

#endif // PROC_EXCEPTION_HPP
