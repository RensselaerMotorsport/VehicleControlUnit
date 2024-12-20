#include "../../Inc/Scheduler/Task.h"

void TaskInit(Task* task, const Updateable* updateable, int hz) {
    task->updateable = updateable;
    task->hz = hz;
    task->nextExecTime = 0;
}

void TaskExecute(Task* task) {
    if (task->updateable && task->updateable->update) {
        UPDATE(task);
    }
}
