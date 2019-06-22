#include "Timer.h"

namespace Timer_n
{
    Timer::Timer(const TimerHandle &handle, const Timeout &timeout)
    {
        started       = false;

        this->handle  = handle;
        this->timeout = timeout;

        startTime   = 0;
        elapsedTime = 0;
    }

    const TimerHandle &Timer::getHandle() const
    {
        return handle;
    }

    const Timeout &Timer::getTimeout() const
    {
        return timeout;
    }

    const StartTime &Timer::getStartTime() const
    {
        return startTime;
    }

    const ElapsedTime &Timer::getElapsedTime() const
    {
        return elapsedTime;
    }

    void Timer::setTimeout(const Timeout &timeout)
    {
        this->timeout = timeout;
    }

    void Timer::setElapsedTime(const ElapsedTime &elapsedTime)
    {
        this->elapsedTime = elapsedTime;
    }

    const bool &Timer::isStarted() const
    {
        return started;
    }
    
    void Timer::start(const StartTime &startTime)
    {
        this->startTime = startTime;

        elapsedTime = 0;

        started = true;
    }

    void Timer::stop()
    {
        startTime = 0;
        elapsedTime = 0;

        started = false;
    }
}