/*
*    This file contains Logger definitions
*/

#include <chrono>
#include <ctime>
#include <algorithm>
#include "Logger.h"

namespace Logging
{
    Logger::Logger(const std::string &filePath)
    {
        fileStream = std::ofstream(filePath, std::ios::out | std::ios::trunc);
    }

    Logger::Logger()
    {
        auto currTime        = std::chrono::system_clock::now();
        auto currTime_time_t = std::chrono::system_clock::to_time_t(currTime);
        auto timeStr         = std::string(std::ctime(&currTime_time_t));

        timeStr.pop_back();
        std::replace_if(timeStr.begin(), timeStr.end(), [](const char &c) { return ((c == ' ') || (c == ':')); }, '_');

        auto filePath = "Log_" + timeStr + ".log";

        fileStream = std::ofstream(filePath, std::ios::out | std::ios::trunc);
    }

    Logger::~Logger() { }

    Logger &Logger::operator<<(const std::string &trace)
    {
        if (fileStream.good() && fileStream.is_open())
        {
            fileStream << trace;
            fileStream.flush();
        }

        return *this;
    }
}
