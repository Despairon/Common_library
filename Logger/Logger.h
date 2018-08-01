#pragma once

/*
*    This file contains Logger declarations
*/

#include <fstream>
#include <string>

namespace Logging
{
    class Logger
    {
    private:
        std::ofstream fileStream;
    public:
        explicit Logger(const std::string&);
        Logger();
        ~Logger();

        Logger& operator<<(const std::string&);
    };
}