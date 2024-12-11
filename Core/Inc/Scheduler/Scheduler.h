/**
  ******************************************************************************
  * @file           : Scheduler.h
  * @brief          : Scheduler for processing each task
  ******************************************************************************
*/

#ifndef RENSSELAERMOTORSPORT_SCHEDULER_H
#define RENSSELAERMOTORSPORT_SCHEDULER_H

#include <stdbool.h>

#include "../Utils/Updateable.h"
#include "Task.h"
#include "PriorityQueue.h"

#define MAX_UPDATEABLES 10
#define MAX_HZ 1000

typedef struct Scheduler {
    PriorityQueue tasks;
    bool running;
} Scheduler;

/**
 * @brief Initializes the scheduler with the given sensors.
 *
 * @param scheduler   Pointer to the Scheduler structure to initialize.
 * @param updatables Array of pointers to Updatables to be scheduled.
 * @param numUpdatables  Number of sensors in the updatables array.
 */
bool SchedulerInit(Scheduler* scheduler, Updateable* updateables[], 
                   int numUpdatables);

/**
 * @brief Runs the scheduler, executing tasks based on their priority.
 *
 * @param scheduler Pointer to the Scheduler structure to run.
 */
void SchedulerRun(Scheduler* scheduler);

/**
 * @brief Stops the scheduler.
 *
 * @param scheduler Pointer to the Scheduler structure to stop.
 */
void SchedulerStop(Scheduler* scheduler);

#endif // RENSSELAERMOTORSPORT_SCHEDULER_H
