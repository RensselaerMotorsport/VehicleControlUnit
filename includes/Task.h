#ifndef RENSSELAERMOTORSPORT_TASK_H
#define RENSSELAERMOTORSPORT_TASK_H

#include "Sensor.h"

// TODO: Configure board to get high-resolution time
typedef int highResTime;

typedef struct {
    int hz;
    highResTime nextExecTime;
    Sensor* sensor;
} Task;

void TaskInit(Task* task, Sensor* sensor, int hz);
void TaskExecute(Task* task);

#endif
