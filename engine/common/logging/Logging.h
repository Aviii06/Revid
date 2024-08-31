#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mixins/SingeltonMixin.h>
#include <types/Containers.h>

#define RESET   "\033[0m"       // UNIMPORTANT DEBUG LOGS
#define RED     "\033[31m"      // Error
#define YELLOW  "\033[33m"      // Warning
#define GREEN   "\033[32m"      // Info

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
};

class Logger : public SingletonMixin<Logger> {
protected:
    Logger() = default;
    std::unordered_map<LogLevel, std::pair<std::string, std::string>> m_debugMap = {
        { LogLevel::INFO, {GREEN, "INFO"} },
        { LogLevel::WARNING, {YELLOW, "WARNING"} },
        { LogLevel::ERROR, {RED, "ERROR"} },
        { LogLevel::DEBUG, {RESET, "DEBUG"} }
    };

    ~Logger() { m_logFile.close(); }

public:
    void SetLogfile(const String& filename)
    {
        m_logFile.open(filename, std::ios::app);
        if (!m_logFile.is_open()) {
            std::cerr << "Error opening log file." << std::endl;
        }

        m_toFile = true;
    }

    void Log(LogLevel level, const String& message)
    {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        std::ostringstream logEntry;
        auto it = m_debugMap.find(level);
        const auto& [color, levelStr] = it->second;

        logEntry << color << "[" << timestamp << "] "
                 << levelStr << ": " << message
                 << std::endl;

        std::cout << logEntry.str();

        if (m_toFile) {
            m_logFile << logEntry.str();
            m_logFile.flush();
        }
    }

private:
    std::ofstream m_logFile;

    bool m_toFile = false;
};
