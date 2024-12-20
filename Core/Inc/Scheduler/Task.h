#ifndef RENSSELAERMOTORSPORT_TASK_H
#define RENSSELAERMOTORSPORT_TASK_H

#include "../Utils/Updateable.h"
// FIXME: Configure board to get high-resolution time
typedef int highResTime;

typedef struct {
    int hz;
    highResTime nextExecTime;
    const Updateable* updateable;
} Task;

/**
 * @brief Initializes a task with the given updateable and update frequency.
 *
 * @param task Pointer to the Task structure to initialize.
 * @param updateable Pointer to the Updateable associated with the task.
 * @param hz The update frequency in Hertz.
 */
void TaskInit(Task* task, const Updateable* updateable, int hz);

/**
 * @brief Executes the task by calling the updateable's update function.
 *
 * @param task Pointer to the Task structure to execute.
 */
void TaskExecute(Task* task);

#endif // RENSSELAERMOTORSPORT_TASK_H
