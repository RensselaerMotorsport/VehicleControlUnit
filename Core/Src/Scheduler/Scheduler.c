#include <time.h>
#include <stdint.h>
#include "../../Inc/Scheduler/Scheduler.h"
// FIXME: Was supposed to be timer include.
// #include "../../Inc/stm32f7xx_hal_conf.h"

// Timer flag declared in the interrupt handler
extern volatile uint32_t timer_flag;

/**
 * Initializes the priority queue and schedules tasks based on the given sensors
 * and their update frequencies. Limits the number of sensors to MAX_SENSORS.
 */
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
        if (sensor == NULL) {
            continue;
        }
        TaskInit(&task, sensor, sensor->updateable.hz);
        int hz = 1000 / sensor->updateable.hz;
        int initialPriority = clock() / (CLOCKS_PER_SEC / 1000) + hz;
        PQPush(&scheduler->tasks, task, initialPriority);
    }
}

/**
 * Runs the scheduler, repeatedly executing tasks based on their scheduled
 * execution times. Uses Sleep or usleep to avoid busy-waiting if no tasks are
 * ready to execute.
 */
void SchedulerRun(Scheduler* scheduler) {
    scheduler->running = true;
    Task currentTask;

    while (scheduler->running) {
        if (timer_flag < 0) continue;
        timer_flag--;
        // FIXME: Re-implement timer.
        // int currentTime = HAL_GetTick();
        int currentTime = 0;

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
