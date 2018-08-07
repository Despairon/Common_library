#pragma once

/*
* This file contains SoC Flasher Timer declarations
*/

#include <functional>
#include <thread>
#include <mutex>

namespace Timer
{
    using TimerTimeoutCallback = std::function<void()>;     // register type name association (timer callback)

    class Timer
    {
    private:
        bool                 running;                       // is timer running flag
        int                  timeout;                       // timer timeout in milliseconds
        TimerTimeoutCallback callback;                      // timer callback
        std::thread          thread;                        // timer thread
        std::mutex           mutex;                         // timer mutes

        static void timerTick(Timer&);                      // timer thread method
    public:
        explicit Timer(const int&, TimerTimeoutCallback);   // constructor
        ~Timer();                                           // destructor

        void start();                                       // start timer method
        void stop();                                        // stop timer method
        
        bool isRunning() const;                             // get is timer running parameter
        
        const int &getTimeout() const;                      // get 'timeout' parameter      
        void setTimeout(const int&);                        // set 'timeout' parameter
    };
}

