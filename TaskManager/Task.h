#pragma once

namespace TaskManager_n
{
    enum Priority: unsigned char
    {
        LOW,
        MEDIUM,
        HIGH
    };

    class Task
    {
    private:
        unsigned int id;
        Priority priority;
    public:
        Task();

        virtual void init() = 0;
        virtual void go() = 0;
        virtual void terminate() = 0;
        
        const unsigned int &getId() const;
        const Priority &getPriority() const;

        void setId(const unsigned int&);
        void setPriority(const Priority&);
    };
}