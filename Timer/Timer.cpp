/*
* This file contains Timer definitions
*/

#include <chrono>
#include "Timer.h"

using namespace std::chrono;

namespace Timer
{
    Timer::Timer(const int &timeout, TimerTimeoutCallback callback)
    {
        running = false;
        this->timeout  = timeout;
        this->callback = callback;
    }

    Timer::~Timer()
    {
        running = false;
        thread.join();
    }

    void Timer::timerTick(Timer &timer)
    {
        static auto startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        auto currTime = startTime;

        while (timer.running)
        {
            if ((currTime - startTime) < std::chrono::milliseconds(timer.timeout))
            {
                currTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
                continue;
            }

            timer.mutex.lock();
            {
                timer.running = false;
            }
            timer.mutex.unlock();

            if (timer.callback)
                timer.callback();
        }
    }

    void Timer::start()
    {
        mutex.lock();
        {
            if (!running)
            {
                running = true;
                thread = std::thread(timerTick, std::ref(*this));
            }
        }
        mutex.unlock();
    }

    void Timer::stop()
    {
        mutex.lock();
        {
            if (running)
            {
                running = false;
                thread.join();
            }
        }
        mutex.unlock();
    }

    bool Timer::isRunning() const
    {
        return running;
    }

    const int &Timer::getTimeout() const
    {
        return timeout;
    }

    void Timer::setTimeout(const int &timeout)
    {
        this->timeout = timeout;
    }
}
