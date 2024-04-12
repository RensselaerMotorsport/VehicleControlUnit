#include "../../includes/Scheduler.h"
// TODO: Configure Clock on board
#include <time.h>
// FIXME: Figure this mess out
#ifdef _WIN32
#include <windows.h>  // For Sleep() on Windows
#else
#include <unistd.h>   // For usleep() on Unix/Linux
#endif

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
    int sleepTime = 1000 / MAX_HZ; // Because Windows

    while (scheduler->running) {
        int currentTime = clock() / (CLOCKS_PER_SEC / 1000);
        bool executed = false;
        
        while (!PQIsEmpty(&scheduler->tasks) && PQPeek(&scheduler->tasks, &currentTask) && currentTask.nextExecTime <= currentTime) {
            if (PQPop(&scheduler->tasks, &currentTask)) {
                TaskExecute(&currentTask); // TODO: May have to reavulate how this works
                // Re-schedule task by updating its next execution time
                currentTask.nextExecTime = currentTime + (1000 / currentTask.hz);
                PQPush(&scheduler->tasks, currentTask, currentTask.nextExecTime);  // Reinsert task with new priority
                executed = true;
            }
        }

        if (!executed) {
            // Sleep to avoid busy-waiting
            #ifdef _WIN32
            Sleep(sleepTime);
            #else
            usleep(MAX_HZ);
            #endif
        }
    }
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = 0;
}
