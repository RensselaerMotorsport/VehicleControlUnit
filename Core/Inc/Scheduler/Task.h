#ifndef RENSSELAERMOTORSPORT_TASK_H
#define RENSSELAERMOTORSPORT_TASK_H

#include "../Utils/Updateable.h"
// FIXME: Configure board to get high-resolution time
typedef int highResTime;

typedef struct {
    int hz;
    highResTime nextExecTime;
    Updateable* updateable;
} Task;

/**
 * @brief Initializes a task with the given sensor and update frequency.
 *
 * @param task    Pointer to the Task structure to initialize.
 * @param updateable Pointer to the Updateable structure associated with the task.
 * @param hz      The update frequency in Hertz.
 * @details This function sets up a task by assigning the specified sensor and update frequency. It also initializes the next execution time to zero.
 */
void TaskInit(Task* task, Updateable* updateable, int hz);

/**
 * @brief Executes the task by calling the sensor's update function.
 *
 * @param task Pointer to the Task structure to execute.
 * @details This function executes the task by invoking the update function of the associated sensor, if it exists.
 */
void TaskExecute(Task* task);

#endif // RENSSELAERMOTORSPORT_TASK_H
