#include <algorithm>

#include "TaskManager.h"

namespace TaskManager_n
{
    TaskManager::TaskManager()
    {
        init();
    }

    TaskManager::~TaskManager()
    {
        terminate();
    }

    void TaskManager::init()
    {
        currentTaskId = 0;
    }

    void TaskManager::go()
    {
        if (!tasks.empty())
        {
            for (auto task : tasks)
                task->go();
        }
    }

    void TaskManager::terminate()
    {
        if (!tasks.empty())
        {
            for (auto task : tasks)
                task->terminate();

            tasks.clear();
        }
    }

    TaskId TaskManager::scheduleTask(Task *task, const Priority &priority)
    {
        if (task != nullptr)
        {
            if (std::find(tasks.begin(), tasks.end(), task) == tasks.end())
            {
                task->setId(currentTaskId++);
                task->setPriority(priority);
                tasks.push_back(task);

                tasks.sort([](Task* first, Task* second)
                {
                    return first->getPriority() >= second->getPriority();
                });

                task->init();

                return task->getId();
            }
            else return 0;
        }
        else return 0;
    }

    void TaskManager::unscheduleTask(Task *task)
    {
        if ((task != nullptr) && !tasks.empty())
        {
            auto taskIter = std::find(tasks.begin(), tasks.end(), task);
            if (taskIter != tasks.end())
            {
                (*taskIter)->terminate();
                tasks.erase(taskIter);
            }
        }
    }
}