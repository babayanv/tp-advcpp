#include "log/loggers/FileLogger.hpp"


namespace log
{


FileLogger::FileLogger(const std::string& file_path, Level lv, Mod mod)
    : BaseLogger(lv, mod)
    , m_ofs(file_path)
{
}

} // namespace log
