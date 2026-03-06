#include "../include/Log.h"
#include <iostream>

namespace GGE
{

    void Log::Info(const std::string& message) {
        Print(LogLevel::Info, message);
    }

    void Log::Warn(const std::string& message) {
        Print(LogLevel::Warn, message);
    }

    void Log::Error(const std::string& message) {
        Print(LogLevel::Error, message);
    }

    void Log::Debug(const std::string& message) {
    #ifdef OSE_DEBUG
        Print(LogLevel::Debug, message);
    #endif
    }

    void Log::Print(LogLevel level, const std::string& message) {
        switch (level) {
            case LogLevel::Info:
                std::cout << "[INFO] ";
                break;
            case LogLevel::Warn:
                std::cout << "[WARN] ";
                break;
            case LogLevel::Error:
                std::cout << "[ERROR] ";
                break;
            case LogLevel::Debug:
                std::cout << "[DEBUG] ";
                break;
        }

        std::cout << message << std::endl;
    }

}
