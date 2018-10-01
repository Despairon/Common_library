#pragma once

#include <list>
#include "Timer.h"

namespace Timer
{
    class TimerManager
    {
        using TimerList = std::list<Timer*>;
        using TimerIterator = TimerList::iterator;

    private:
        TimerList timerList;

        TimerIterator findTimer(const std::string&);
    public:
        explicit TimerManager();
        ~TimerManager();

        void createTimer(const std::string&, const int&, TimerTimeoutCallback);
        void deleteTimer(const std::string&);
        void startTimer(const std::string&);
        void stopTimer(const std::string&);
        void changeTimerTimeout(const std::string&, const int&);
        const bool &isTimerActive(const std::string&);

    };
}