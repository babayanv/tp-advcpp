#ifndef LOG_FORMATTER_HPP
#define LOG_FORMATTER_HPP

#include "log/format/BaseFormatter.hpp"

#include <sstream>


namespace log
{


class LogFormatter final : public BaseFormatter
{
public:
    LogFormatter(Mod mod);

    std::string fmt(const std::string& msg, Level lv) noexcept override;

private:
    void handleColor(std::ostringstream& oss, Level lv);
    void handleTime(std::ostringstream& oss);
    void handleWeight(std::ostringstream& oss, Level lv);
};

} // namespace log

#endif // LOG_FORMATTER_HPP
