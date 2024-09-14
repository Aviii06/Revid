#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <types/Containers.h>

namespace Revid
{
	enum class LogLevel {
		DEBUG = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
	};
}

namespace Revid::Logger
{
	static std::ofstream s_logFile;
	static bool s_toFile = false;

	#define RESET   "\033[0m"       // UNIMPORTANT DEBUG LOGS
	#define RED     "\033[31m"      // Error
	#define YELLOW  "\033[33m"      // Warning
	#define GREEN   "\033[32m"      // Info

	static const std::unordered_map<const LogLevel, std::pair<const std::string, const std::string>> m_debugMap = {
		{ LogLevel::INFO, {GREEN, "INFO"} },
		{ LogLevel::WARNING, {YELLOW, "WARNING"} },
		{ LogLevel::ERROR, {RED, "ERROR"} },
		{ LogLevel::DEBUG, {RESET, "DEBUG"} }
	};

	static void SetLogfile(const String& filename)
	{
		s_logFile.open(filename, std::ios::app);
		if (!s_logFile.is_open())
		{
			std::cerr << "Error opening log file." << std::endl;
		}

		s_toFile = true;
	}

	static void Log(const LogLevel& level, const String& message)
	{
#ifdef NDEBUG
		return;
#else
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

		if (s_toFile)
		{
			s_logFile << logEntry.str();
			s_logFile.flush();
		}
#endif
	}
}
