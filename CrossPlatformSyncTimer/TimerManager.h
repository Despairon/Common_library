#pragma once

#include <list>
#include <functional>

#include "Timer.h"
#include "Task.h"
#include "EventSystem.h"

namespace TimerManager_n
{
    constexpr auto INVALID_TIMER_HANDLE = (unsigned int)(-1);

    using namespace Timer_n;
    using namespace TaskManager_n;
    using namespace EventSystem_n;

    using TimersList  = std::list<Timer>;

    using CurrentTimeGetter = std::function<unsigned long long()>;

    class TimerManager : public Task
    {
    private:
        EventSystem      *eventSystem;
        EventId           timerEvent;
        TimerHandle       currentTimerHandle;
        bool              correctlyInitialized;
        TimersList        timersList;
        CurrentTimeGetter currentTimeGetter;

    public:
        explicit TimerManager(EventSystem*, const EventId&, CurrentTimeGetter);
        ~TimerManager();

        void init() override;
        void go() override;
        void terminate() override;

        TimerHandle createTimer(const unsigned int&);
        void startTimer(const TimerHandle&);
        void restartTimer(const TimerHandle&);
        void stopTimer(const TimerHandle&);
        void removeTimer(const TimerHandle&);
        bool isTimerActive(const TimerHandle&) const;
        void stopAllTimers();
    };
}