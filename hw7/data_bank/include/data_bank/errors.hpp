#ifndef DATA_BANK_ERRORS_HPP
#define DATA_BANK_ERRORS_HPP

#include <exception>
#include <string>


namespace data_bank
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


class FileError : public ErrnoException
{
public:
    FileError() = default;
    explicit FileError(const std::string& what_arg) : ErrnoException(what_arg) {}
};


class CacheFileError : public std::exception
{
public:
    explicit CacheFileError(std::string what_arg);

    ~CacheFileError() noexcept override = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};


} // namespace data_bank

#endif // DATA_BANK_ERRORS_HPP