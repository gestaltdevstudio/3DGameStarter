#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <string>

namespace GGE {

    enum class LogLevel {
        Info,
        Warn,
        Error,
        Debug
    };

    class Log {
    public:
        static void Info(const std::string& message);
        static void Warn(const std::string& message);
        static void Error(const std::string& message);
        static void Debug(const std::string& message);

    private:
        static void Print(LogLevel level, const std::string& message);
    };

}

#endif // LOG_H_INCLUDED
