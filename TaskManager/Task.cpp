#include "Task.h"

namespace TaskManager_n
{
    Task::Task()
    {
        id = 0;
        priority = Priority::MEDIUM;
    }

    const Priority &Task::getPriority() const
    {
        return priority;
    }

    const unsigned int &Task::getId() const
    {
        return id;
    }

    void Task::setId(const unsigned int &id)
    {
        this->id = id;
    }

    void Task::setPriority(const Priority &priority)
    {
        this->priority = priority;
    }
}