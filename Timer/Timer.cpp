/*
* This file contains Timer definitions
*/

#include <chrono>
#include "Timer.h"

using namespace std::chrono;

namespace Timer
{
    Timer::Timer(const std::string &name, const int &timeout, TimerTimeoutCallback callback) :
        running(false),
        name(name),
        timeout(timeout),
        callback(callback)
    {
    }

    Timer::~Timer()
    {
        if (thread.joinable())
        {
            running = false;
            thread.join();
        }
    }

    void Timer::timerTick(Timer &timer)
    {
        auto startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
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
            if (thread.joinable())
                thread.join();

            running = true;
            thread = std::thread(timerTick, std::ref(*this));
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

    const bool &Timer::isRunning() const
    {
        return running;
    }

    const std::string &Timer::getName() const
    {
        return name;
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
