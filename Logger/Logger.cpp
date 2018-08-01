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
#ifdef LOGGING_ENABLED // TODO: remove when released
        fileStream = std::ofstream(filePath, std::ios::out | std::ios::trunc);
#endif
    }

    Logger::Logger()
    {
        auto currTime        = std::chrono::system_clock::now();
        auto currTime_time_t = std::chrono::system_clock::to_time_t(currTime);
        auto timeStr         = std::string(std::ctime(&currTime_time_t));

        timeStr.pop_back();
        std::replace_if(timeStr.begin(), timeStr.end(), [](const char &c) { return ((c == ' ') || (c == ':')); }, '_');

        auto filePath = "Log_" + timeStr + ".log";

#ifdef LOGGING_ENABLED // TODO: remove when released
        fileStream = std::ofstream(filePath, std::ios::out | std::ios::trunc);
#endif
    }

    Logger::~Logger() { }

    Logger &Logger::operator<<(const std::string &trace)
    {
        if (fileStream.good() && fileStream.is_open())
        {
#ifdef LOGGING_ENABLED // TODO: remove when released
            fileStream << trace;
#endif
        }

        return *this;
    }
}