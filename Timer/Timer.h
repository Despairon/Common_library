#pragma once

/*
* This file contains Timer declarations
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
        std::mutex           mutex;                         // timer mutex
        std::string          name;                          // timer name to identify it

        static void timerTick(Timer&);                      // timer thread method
    public:
        explicit Timer(const std::string&, 
                       const int&, 
                       TimerTimeoutCallback);               // constructor
        ~Timer();                                           // destructor

        void start();                                       // start timer method
        void stop();                                        // stop timer method
        
        const bool &isRunning() const;                      // get is timer running parameter
        const std::string &getName() const;                 // get timer name
        
        const int &getTimeout() const;                      // get 'timeout' parameter      
        void setTimeout(const int&);                        // set 'timeout' parameter
    };
}

