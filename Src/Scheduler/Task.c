#include "../../includes/Task.h"

void TaskInit(Task* task, Sensor* sensor, int hz) {
    task->sensor = sensor;
    task->hz = hz;
    task->nextExecTime = 0;
}

void TaskExecute(Task* task) {
    if (task->sensor && task->sensor->updateable.update) {
        UPDATE(task->sensor);
    }
}
