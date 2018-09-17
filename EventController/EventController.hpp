#pragma once

/*
* This file contains Event Controller declarations and definitions
*/

#include <mutex>
#include <thread>
#include <queue>
#include <map>
#include <functional>

namespace EventManager
{
    template<typename EventType, typename EventDataType>
    class EventController
    {
        using EventHandlerType = std::function<void(EventDataType)>;
        using EventsQueue      = std::queue<std::pair<EventType, EventDataType>>;
        using EventsMap        = std::map<EventType, EventHandlerType>;
    private:
        bool        running;
        std::thread thread;
        std::mutex  mutex;
        EventsQueue eventsQueue;
        EventsMap   eventsMap;

        static void eventLoop(EventController &controller)                                       // event loop thread method
        {
            while (controller.running)
            {
                if (!controller.eventsQueue.empty())
                {
                    controller.mutex.lock();

                    auto eventToHandle = controller.eventsQueue.front();

                    controller.eventsQueue.pop();

                    controller.mutex.unlock();

                    if (controller.eventsMap.find(eventToHandle.first) != controller.eventsMap.end())
                    {
                        if (controller.eventsMap.at(eventToHandle.first) != 0)
                        {
                            controller.eventsMap.at(eventToHandle.first)(eventToHandle.second);
                        }
                    }
                }
            }
        }

    public:
        explicit EventController()                                                          // constructor
        {
            running = true;
            thread = std::thread(eventLoop, std::ref(*this));
        }

        ~EventController()                                                                  // destructor
        {
            if (running)
            {
                running = false;

                if (thread.joinable())
                    thread.join();
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
    };
}