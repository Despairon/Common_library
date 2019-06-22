#pragma once

#include <queue>
#include <map>
#include <list>

#include "EventSubscriber.h"
#include "Task.h"

namespace EventSystem_n
{
    using namespace TaskManager_n;

    class EventSystem : public Task
    {
        using SubscribersList  = std::list<EventSubscriber*>;
        using Subscriptions    = std::map<EventId, SubscribersList>;
        using EventQueue       = std::queue<Event>;

    private:
        Subscriptions subscriptions;
        EventQueue    eventQueue;
        EventId       eventId;

    public:
        EventSystem();
        ~EventSystem();

        EventId createEvent();
        void subscribe(const EventId&, EventSubscriber*);
        void unsubscribe(const EventId&, EventSubscriber*);
        void sendEvent(const Event&);

        void init() override;
        void go() override;
        void terminate() override;
    };
}