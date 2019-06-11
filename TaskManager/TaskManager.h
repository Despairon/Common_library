#pragma once

#include <list>

#include "Task.h"

namespace TaskManager_n
{
    using TaskId = unsigned int;

    class TaskManager : public Task
    {
    private:
        TaskId currentTaskId;
        std::list<Task*> tasks;
    public:
        TaskManager();
        ~TaskManager();

        void init() override;
        void go() override;
        void terminate() override;

        TaskId scheduleTask(Task *task, const Priority &priority = Priority::MEDIUM);
        void unscheduleTask(Task *task);
    };
}