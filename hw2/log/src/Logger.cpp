#include "log/Logger.hpp"


namespace log
{


Logger& Logger::get_instance()
{
    static Logger s_instance;
    return s_instance;
}

} // namespace log
