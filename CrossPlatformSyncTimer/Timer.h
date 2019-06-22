#pragma once

namespace Timer_n
{
    using TimerHandle = unsigned int;
    using Timeout     = unsigned int;
    using StartTime   = unsigned long long;
    using ElapsedTime = unsigned long long;

    class Timer
    {
    private:
        bool        started;
        TimerHandle handle;
        Timeout     timeout;
        StartTime   startTime;
        ElapsedTime elapsedTime;

    public:
        explicit Timer(const TimerHandle&, const Timeout&);

        const TimerHandle &getHandle() const;
        const Timeout     &getTimeout() const;
        const StartTime   &getStartTime() const;
        const ElapsedTime &getElapsedTime() const;

        void setTimeout(const Timeout&);
        void setElapsedTime(const ElapsedTime&);

        const bool &isStarted() const;
        
        void start(const StartTime&);
        void stop();
    };
}