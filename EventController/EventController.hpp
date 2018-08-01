#pragma once

/*
* This file contains Event Controller declarations and definitions
*/

#include <mutex>
#include <queue>
#include <map>
#include <windows.h>

namespace EventManager
{
    template<typename EventType, typename EventDataType, typename EventHandlerType>
    class EventController
    {
    private:
        bool                                            initialized;
        HANDLE                                          threadHandle;
        std::mutex                                      mutex;
        std::queue<std::pair<EventType, EventDataType>> eventsQueue;
        std::map<EventType, EventHandlerType>           eventsMap;

        static DWORD WINAPI eventLoop(LPVOID lpParam)                                       // event loop thread method
        {
            if (lpParam)
            {
                auto controller = static_cast<EventController*>(lpParam);

                while (true)
                {
                    if (controller->isInitialized())
                    {
                        if (!controller->eventsQueue.empty())
                        {
                            controller->mutex.lock();

                            auto eventToHandle = controller->eventsQueue.front();

                            controller->eventsQueue.pop();

                            controller->mutex.unlock();

                            if (controller->eventsMap.find(eventToHandle.first) != controller->eventsMap.end())
                            {
                                if (controller->eventsMap.at(eventToHandle.first) != 0)
                                {
                                    controller->eventsMap.at(eventToHandle.first)(eventToHandle.second);
                                }
                            }
                        }
                    }
                }
            }

            return EXIT_SUCCESS;
        }

    public:
        explicit EventController()                                                          // constructor
        { 
            initialized = false;

            threadHandle = CreateThread(NULL, 0, eventLoop, this, NULL, NULL);

            if (threadHandle)
                initialized = true;
            else
                throw std::exception("Unknown event controller thread creation error");
        }

        ~EventController()                                                                  // destructor
        { 
            if (threadHandle)
            {
                TerminateThread(threadHandle, EXIT_SUCCESS);
                CloseHandle(threadHandle);
            }
        }

        void registerEvent(const EventType &event, EventHandlerType eventHandler)           // registers event in the event loop
        {
            mutex.lock();

            if (eventsMap.find(event) == eventsMap.end())
                eventsMap[event] = eventHandler;

            mutex.unlock();
        }

        void raiseEvent(const EventType &event, const EventDataType &eventData)             // enqueues certain event
        {
            mutex.lock();

            eventsQueue.push(std::pair<EventType, EventDataType>(event, eventData));

            mutex.unlock();
        }

        const bool &isInitialized() const                                                   // 'initialized' parameter getter
        {
            return initialized;
        }
    };

}