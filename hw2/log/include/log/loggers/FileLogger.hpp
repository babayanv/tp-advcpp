#ifndef FILE_LOGGER_HPP
#define FILE_LOGGER_HPP

#include "log/loggers/BaseLogger.hpp"

#include <fstream>
#include <memory>


namespace log
{

class FileLogger final : public BaseLogger
{
public:
    FileLogger(const std::string& file_path, Level lv, Mod mod);

    FileLogger(const FileLogger& other) = delete;
    FileLogger& operator=(const FileLogger& other) = delete;

    FileLogger(FileLogger&& other) = delete;
    FileLogger& operator=(FileLogger&& other) = delete;

    virtual void flush() noexcept override;

private:
    std::ofstream m_ofs;

private:
    virtual void log_impl(const std::string& log_msg) noexcept override;

};


std::shared_ptr<FileLogger> create_file_logger(const std::string& file_path, Level lv, Mod mod = Mod::TIME) noexcept;

} // namespace log

#endif // FILE_LOGGER_HPP
