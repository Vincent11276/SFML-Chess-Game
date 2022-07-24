// Copy-pasted from https://stackoverflow.com/questions/22372032/resource-manager
// Logger class inspired by https://github.com/ThiccTheo

#pragma once

#include <sstream>
#include <iostream>

enum class LogLevel
{
    NO = 0,
    ERROR = 1,
    INFO = 2,
    DEBUG = 3
};

class Logger
{
public:
    static Logger& getInstance();

    void setLogLevel(const LogLevel& logLevel);

    void log(const LogLevel& messageLevel, const std::string& message);


private:
    Logger();

    ~Logger();

    void setPrefix(const LogLevel& messageLevel);

    std::string logPrefix_;
    LogLevel currentLogLevel_;
    LogLevel messageLevel_;

};

