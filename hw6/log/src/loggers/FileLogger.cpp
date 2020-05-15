#include "log/loggers/FileLogger.hpp"


namespace log
{


FileLogger::FileLogger(const std::string& file_path, Level lv, Mod mod)
    : BaseLogger(lv, mod)
    , m_ofs(file_path)
{
}


void FileLogger::flush() noexcept
{
    m_ofs.flush();
}


void FileLogger::log_impl(const std::string& log_msg) noexcept
{
    m_ofs << log_msg << std::endl;
}


std::shared_ptr<FileLogger> create_file_logger(const std::string& file_path, Level lv, Mod mod) noexcept
{
    return std::make_shared<FileLogger>(file_path, lv, mod);
}

} // namespace log
