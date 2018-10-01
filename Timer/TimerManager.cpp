#include <algorithm>
#include "TimerManager.h"

namespace Timer
{
    TimerManager::TimerManager()
    {
    }

    TimerManager::~TimerManager()
    {
        if (!timerList.empty())
        {
            for (auto timer = timerList.begin(); timer != timerList.end(); timer++)
            {
                if ((*timer) != nullptr)
                {
                    if ((*timer)->isRunning())
                    {
                        (*timer)->stop();
                    }

                    delete (*timer);
                    (*timer) = nullptr;
                }
            }

            timerList.clear();
        }
    }

    TimerManager::TimerIterator TimerManager::findTimer(const std::string &timerName)
    {
        auto timer = std::find_if(timerList.begin(), timerList.end(), [timerName](auto element) -> bool
        {
            return (element->getName() == timerName);
        });

        return timer;
    }

    void TimerManager::createTimer(const std::string &timerName, const int &timerTimeout, TimerTimeoutCallback timerCallback)
    {
        auto timer = findTimer(timerName);
        if (timer == timerList.end())
        {
            auto timer = new Timer(timerName, timerTimeout, timerCallback);

            timerList.push_back(timer);
        }
        else
        {
            throw std::exception(std::string("Timer \"" + timerName + "\" is already created!").c_str());
        }
    }

    void TimerManager::deleteTimer(const std::string &timerName)
    {
        auto timer = findTimer(timerName);
        if (timer != timerList.end())
        {
            if ((*timer)->isRunning())
            {
                (*timer)->stop();
            }

            auto timerPtr = (*timer);

            timerList.remove(*timer);

            delete timerPtr;
        }
        else
        {
            throw std::exception(std::string("No such timer: \"" + timerName + "\"").c_str());
        }
    }

    void TimerManager::startTimer(const std::string &timerName)
    {
        auto timer = findTimer(timerName);
        if (timer != timerList.end())
        {
            if (!(*timer)->isRunning())
            {
                (*timer)->start();
            }
        }
        else
        {
            throw std::exception(std::string("No such timer: \"" + timerName + "\"").c_str());
        }
    }

    void TimerManager::stopTimer(const std::string &timerName)
    {
        auto timer = findTimer(timerName);
        if (timer != timerList.end())
        {
            if ((*timer)->isRunning())
            {
                (*timer)->stop();
            }
        }
        else
        {
            throw std::exception(std::string("No such timer: \"" + timerName + "\"").c_str());
        }
    }

    void TimerManager::changeTimerTimeout(const std::string &timerName, const int &timerTimeout)
    {
        auto timer = findTimer(timerName);
        if (timer != timerList.end())
        {
            (*timer)->setTimeout(timerTimeout);
        }
        else
        {
            throw std::exception(std::string("No such timer: \"" + timerName + "\"").c_str());
        }
    }

    const bool &TimerManager::isTimerActive(const std::string &timerName)
    {
        auto timer = findTimer(timerName);
        if (timer != timerList.end())
        {
            return (*timer)->isRunning();
        }
        else
        {
            throw std::exception(std::string("No such timer: \"" + timerName + "\"").c_str());
        }
    }
}