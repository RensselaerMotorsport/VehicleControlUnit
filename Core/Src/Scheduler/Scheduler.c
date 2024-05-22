#include <time.h>
#include "../../Inc/Scheduler/Scheduler.h"
#include "../stm32f7xx_hal.h"

// Timer flag declared in the interrupt handler
extern volatile uint32_t timer_flag;

void SchedulerInit(Scheduler* scheduler, Sensor* sensorArray[], int numSensors) {
    if (numSensors > MAX_SENSORS) {
        printf("Warning: Number of sensors exceeds MAX_SENSORS. Some sensors will not be scheduled.\n");
        numSensors = MAX_SENSORS;  // Limit the number of sensors to MAX_SENSORS
    }

    PQInit(&scheduler->tasks);
    scheduler->running = false;

    for (int i = 0; i < numSensors; i++) {
        Task task;
        Sensor* sensor = sensorArray[i];
        if (sensor != NULL) {
            TaskInit(&task, sensor, sensor->hz);
            int initialPriority = clock() / (CLOCKS_PER_SEC / 1000) + (1000 / sensor->hz);
            PQPush(&scheduler->tasks, task, initialPriority);
        }
    }
}


void SchedulerRun(Scheduler* scheduler) {
    scheduler->running = true;
    Task currentTask;

    while (scheduler->running) {
        if (timer_flag < 0) continue;
        timer_flag--;
        int currentTime = HAL_GetTick();

        while (!PQIsEmpty(&scheduler->tasks) &&
               PQPeek(&scheduler->tasks, &currentTask) &&
               currentTask.nextExecTime <= currentTime) {
            if (PQPop(&scheduler->tasks, &currentTask)) {
                TaskExecute(&currentTask);
                currentTask.nextExecTime = currentTime + (1000 / currentTask.hz);
                PQPush(&scheduler->tasks, currentTask, currentTask.nextExecTime);  // Reinsert task with new priority
            }
        }
    }
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = 0;
}
