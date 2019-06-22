#include "EventSystem.h"

namespace EventSystem_n
{
    EventSystem::EventSystem()
    {
        init();
    }

    EventSystem::~EventSystem()
    {
        terminate();
    }

    EventId EventSystem::createEvent()
    {
        return eventId++;
    }

    void EventSystem::subscribe(const EventId &event, EventSubscriber *subscriber)
    {
        if (subscriber != nullptr)
        {
            if (subscriptions.find(event) == subscriptions.end())
                subscriptions[event] = SubscribersList();

            if (std::find(subscriptions[event].begin(), subscriptions[event].end(), subscriber) == subscriptions[event].end())
                subscriptions[event].push_back(subscriber);
        }
    }

    void EventSystem::unsubscribe(const EventId &event, EventSubscriber *subscriber)
    {
        if (subscriber != nullptr)
        {
            if (subscriptions.find(event) != subscriptions.end())
                subscriptions[event].remove(subscriber);
        }
    }

    void EventSystem::sendEvent(const Event &event)
    {
        eventQueue.push(event);
    }

    void EventSystem::init()
    {
        // do nothing for now
    }

    void EventSystem::go()
    {
        if (!eventQueue.empty())
        {
            auto event = eventQueue.front();

            if (subscriptions.find(event.getId()) != subscriptions.end())
            {
                for (auto subscriber : subscriptions[event.getId()])
                    if (subscriber != nullptr)
                        subscriber->onEvent(this, event);
            }

            eventQueue.pop();
        }
    }

    void EventSystem::terminate()
    {
        if (!subscriptions.empty())
            subscriptions.clear();
    }
}