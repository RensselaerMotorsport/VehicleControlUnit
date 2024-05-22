#ifndef RENSSELAERMOTORSPORT_SCHEDULER_H
#define RENSSELAERMOTORSPORT_SCHEDULER_H

#include <stdbool.h>
#include "Sensor.h"
#include "Task.h"
#include "PriorityQueue.h"

#define MAX_SENSORS 10
#define MAX_HZ 1000


typedef struct Scheduler {
    PriorityQueue tasks;
    bool running;
} Scheduler;

void SchedulerInit(Scheduler* scheduler, Sensor* sensorArray[], int numSensors);
void SchedulerRun(Scheduler* scheduler);
void SchedulerStop(Scheduler* scheduler);

#endif
