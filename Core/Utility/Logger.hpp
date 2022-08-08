// Credits to https://github.com/ThiccTheo

#pragma once

#include <iostream>
#include <format>
#include <source_location>
#include <string>

struct FmtString
{
	std::string_view message;
	std::source_location location;

	FmtString();
	FmtString(const char* message, const std::source_location& location = std::source_location::current());
	FmtString(const std::string& message, const std::source_location& location = std::source_location::current());
};

class Logger
{
public:
	enum class LogLevel { debug, info, warning, error, critical, count };

	template<typename... T>
	static void debug(const FmtString formatString, const T&... t);

	template<typename... T>
	static void info(const FmtString formatString, const T&... t);

	template<typename... T>
	static void warning(const FmtString formatString, const T&... t);

	template<typename... T>
	static void error(const FmtString formatString, const T&... t);

	template<typename... T>
	static void critical(const FmtString formatString, const T&... t);

	static void clear();
private:
	static void print(const LogLevel logLevel, std::string message, const std::source_location& location);
};

template<typename... T>
static void Logger::debug(const FmtString formatString, const T&... t)
{
	print(LogLevel::debug, std::vformat(formatString.message, std::make_format_args(t...)), formatString.location);
}

template<typename... T>
static void Logger::info(const FmtString formatString, const T&... t)
{
	print(LogLevel::info, std::vformat(formatString.message, std::make_format_args(t...)), formatString.location);
}

template<typename... T>
static void Logger::warning(const FmtString formatString, const T&... t)
{
	print(LogLevel::warning, std::vformat(formatString.message, std::make_format_args(t...)), formatString.location);
}

template<typename... T>
static void Logger::error(const FmtString formatString, const T&... t)
{
	print(LogLevel::error, std::vformat(formatString.message, std::make_format_args(t...)), formatString.location);
}

template<typename... T>
static void Logger::critical(const FmtString formatString, const T&... t)
{
	print(LogLevel::critical, std::vformat(formatString.message, std::make_format_args(t...)), formatString.location);
}