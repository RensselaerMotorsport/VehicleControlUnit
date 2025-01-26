#include "../../Inc/Scheduler/Task.h"
#include "../../Inc/Systems/PrintHelpers.h"

void TaskInit(Task* task, const Updateable* updateable, int hz) {
    task->updateable = updateable;
    task->hz = hz;
    task->nextExecTime = 0;
}

void TaskExecute(Task* task) {
    // Guard Clauses for UPDATE
    if (task->updateable == NULL) {
        printf(ANSI_COLOR_RED "Error: Task has NULL updateable.\n" ANSI_COLOR_RESET);
        return;
    }
    if (task->updateable->update == NULL) {
        printf(ANSI_COLOR_RED "Error: Updateable '%s' has no update function.\n" ANSI_COLOR_RESET,
               task->updateable->name);
        return;
    }
    if (task->updateable->context == NULL) {
        printf(ANSI_COLOR_RED "Error: Updateable '%s' has NULL context.\n" ANSI_COLOR_RESET,
               task->updateable->name);
        return;
    }

    UPDATE(task);

#ifndef STOPLOGGING
    // Perform logging
    if (task->updateable->log != NULL) {
        LOG(task);
    }
#endif
}
