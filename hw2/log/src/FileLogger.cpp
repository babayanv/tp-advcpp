#include "log/FileLogger.hpp"


namespace log
{


FileLogger::FileLogger(const std::string& file_path)
    : m_ofs(file_path)
{
}


FileLogger::FileLogger(FileLogger&& other)
    : m_ofs(std::move(other.m_ofs))
{
}



FileLogger& FileLogger::operator=(FileLogger&& other)
{
    m_ofs = std::move(other.m_ofs);
    return *this;
}


std::shared_ptr<FileLogger> create_file_logger(const std::string& file_path, Level lv)
{
    auto logger = std::make_shared<FileLogger>(file_path);
    logger->set_level(lv);

    return logger;
}

} // namespace log
