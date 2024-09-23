#ifndef RENSSELAERMOTORSPORT_SCHEDULER_H
#define RENSSELAERMOTORSPORT_SCHEDULER_H

#include <stdbool.h>

#include "../Sensors/Sensor.h"
#include "Task.h"
#include "PriorityQueue.h"

#define MAX_SENSORS 10
#define MAX_HZ 1000

typedef struct Scheduler {
    PriorityQueue tasks;
    bool running;
} Scheduler;

/**
 * @brief Initializes the scheduler with the given sensors.
 *
 * @param scheduler   Pointer to the Scheduler structure to initialize.
 * @param sensorArray Array of pointers to Sensor structures to be scheduled.
 * @param numSensors  Number of sensors in the sensorArray.
 */
void SchedulerInit(Scheduler* scheduler, Sensor* sensorArray[], int numSensors);

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
