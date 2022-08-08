// Credits to https://github.com/ThiccTheo

#include "Logger.hpp"

FmtString::FmtString() = default;

FmtString::FmtString(const char* message, const std::source_location& location)
	: message{ message },
	location{ location }
{
}

FmtString::FmtString(const std::string& message, const std::source_location& location)
	: message{ message },
	location{ location }
{
}

void Logger::print(const LogLevel logLevel, std::string message, const std::source_location& location)
{
	switch (logLevel)
	{
	case LogLevel::debug:
		message.insert(0, "[Debug] ");
		break;
	case LogLevel::info:
		message.insert(0, "[Info] ");
		break;
	case LogLevel::warning:
		message.insert(0, "[Warning] ");
		break;
	case LogLevel::error:
		message.insert(0, "[Error] ");
		break;
	case LogLevel::critical:
		message.insert(0, "[Critical] ");
		break;
	}

	message += std::format(" [{}:{}:{}]\n", strrchr(location.file_name(), '\\') ? strrchr(location.file_name(), '\\') + 1 : location.file_name(), location.line(), location.column());

	if (static_cast<int>(logLevel) >= static_cast<int>(LogLevel::warning))
	{
		std::cerr << message;
	}
	else
	{
		std::cout << message;
	}
}

void Logger::clear()
{
	std::cout << "\033[H\033[J";
}