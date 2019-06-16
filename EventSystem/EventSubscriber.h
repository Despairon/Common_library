#pragma once

#include "Event.h"

namespace EventSystem_n
{
    class EventSystem;

    class EventSubscriber
    {
    public:
        virtual void onEvent(EventSystem *sender, const Event &event) = 0;
    };
}