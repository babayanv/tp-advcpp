#ifndef FILE_LOGGER_HPP
#define FILE_LOGGER_HPP

#include "log/BaseLogger.hpp"

#include <fstream>
#include <memory>


namespace log
{

class FileLogger final : public BaseLogger
{
public:
    FileLogger(const std::string& file_path);

    FileLogger(const FileLogger& other) = delete;
    FileLogger& operator=(const FileLogger& other) = delete;

    FileLogger(FileLogger&& other);
    FileLogger& operator=(FileLogger&& other);

    virtual inline void flush() noexcept override
    {
        m_ofs.flush();
    }

private:
    std::ofstream m_ofs;

private:
    virtual inline void log_impl(const std::string& log_msg) noexcept override
    {
        m_ofs << log_msg;
    }

};


std::shared_ptr<FileLogger> create_file_logger(const std::string& file_path, Level lv);

} // namespace log

#endif // FILE_LOGGER_HPP