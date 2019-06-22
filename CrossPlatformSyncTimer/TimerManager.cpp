#include <algorithm>

#include "TimerManager.h"

namespace TimerManager_n
{
    TimerManager::TimerManager(EventSystem *eventSystem, const EventId &timerEvent, CurrentTimeGetter currentTimeGetter)
    {
        if ( (eventSystem != nullptr) && (currentTimeGetter != nullptr) )
        {
            this->eventSystem       = eventSystem;
            this->timerEvent        = timerEvent;
            this->currentTimeGetter = currentTimeGetter;

            correctlyInitialized = true;
        }

        currentTimerHandle = 0;
    }

    TimerManager::~TimerManager()
    {
        if (correctlyInitialized)
            terminate();
    }

    void TimerManager::init()
    {
        // do nothing
    }

    void TimerManager::go()
    {
        if (correctlyInitialized)
        {
            for (auto timer = timersList.begin(); timer != timersList.end(); timer++)
            {
                if (timer->isStarted())
                {
                    if (timer->getElapsedTime() > timer->getTimeout())
                    {
                        auto tmrHandle = timer->getHandle();

                        eventSystem->sendEvent(Event(timerEvent, &tmrHandle, sizeof(tmrHandle)));

                        stopTimer(tmrHandle);
                    }
                    else
                    {
                        timer->setElapsedTime(currentTimeGetter() - timer->getStartTime());
                    }
                }
            }
        }
    }

    void TimerManager::terminate()
    {
        timersList.clear();
    }

    TimerHandle TimerManager::createTimer(const unsigned int &timeout)
    {
        if ( (currentTimerHandle != INVALID_TIMER_HANDLE) && correctlyInitialized)
        {
            if (std::find_if(timersList.begin(), timersList.end(), [this](const Timer &tmr) { return tmr.getHandle() == this->currentTimerHandle; }) == timersList.end())
            {
                timersList.push_back(Timer(currentTimerHandle, timeout));

                return currentTimerHandle++;
            }
            else
                return INVALID_TIMER_HANDLE;
        }
        else
            return INVALID_TIMER_HANDLE;
    }

    void TimerManager::startTimer(const TimerHandle &timerHandle)
    {
        if (correctlyInitialized && (timerHandle != INVALID_TIMER_HANDLE))
        {
            auto timer = std::find_if(timersList.begin(), timersList.end(), [&timerHandle](const Timer &tmr) { return tmr.getHandle() == timerHandle; });

            if (timer != timersList.end())
                if (!timer->isStarted())
                    timer->start(currentTimeGetter());
        }
    }

    void TimerManager::restartTimer(const TimerHandle &timerHandle)
    {
        stopTimer(timerHandle);

        startTimer(timerHandle);
    }

    void TimerManager::stopTimer(const TimerHandle &timerHandle)
    {
        if (correctlyInitialized && (timerHandle != INVALID_TIMER_HANDLE))
        {
            auto timer = std::find_if(timersList.begin(), timersList.end(), [&timerHandle](const Timer &tmr) { return tmr.getHandle() == timerHandle; });

            if (timer != timersList.end())
                if (timer->isStarted())
                    timer->stop();
        }
    }

    void TimerManager::removeTimer(const TimerHandle &timerHandle)
    {
        if (correctlyInitialized && (timerHandle != INVALID_TIMER_HANDLE))
        {
            auto timer = std::find_if(timersList.begin(), timersList.end(), [&timerHandle](const Timer &tmr) { return tmr.getHandle() == timerHandle; });

            if (timer != timersList.end())
                timersList.erase(timer);
        }
    }

    bool TimerManager::isTimerActive(const TimerHandle &timerHandle) const
    {
        if (correctlyInitialized && (timerHandle != INVALID_TIMER_HANDLE))
        {
            auto timer = std::find_if(timersList.begin(), timersList.end(), [&timerHandle](const Timer &tmr) { return tmr.getHandle() == timerHandle; });

            if (timer != timersList.end())
                return timer->isStarted();
            else
                return false;
        }
        else
            return false;
    }

    void TimerManager::stopAllTimers()
    {
        if (correctlyInitialized && !timersList.empty())
            for (auto timer = timersList.begin(); timer != timersList.end(); timer++)
                if (timer->isStarted())
                    stopTimer(timer->getHandle());
    }
}