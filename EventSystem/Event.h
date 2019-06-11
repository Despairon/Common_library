#pragma once

namespace EventSystem_n
{
    using EventId = unsigned char;

    class Event
    {
        EventId id;
        void   *data;
        size_t  size;
    public:
        Event();
        Event(const Event&);
        explicit Event(const EventId &id, void *data = nullptr, const size_t &size = 0);
        ~Event();
        
        Event operator=(const Event&);

        const EventId &getId() const;

        void getData(void *, const size_t&) const;
    };
}