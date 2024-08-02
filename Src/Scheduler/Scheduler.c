#include "../../Inc/Scheduler/Scheduler.h"
// TODO: Configure Clock on board
#include <time.h>

#ifdef _WIN32
#include <windows.h>  // For Sleep() on Windows
#endif

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
    int sleepTime = 1000 / MAX_HZ; // Because Windows

    while (scheduler->running) {
        int currentTime = clock() / (CLOCKS_PER_SEC / 1000);
        bool executed = false;

        while (!PQIsEmpty(&scheduler->tasks) &&
                PQPeek(&scheduler->tasks, &currentTask) &&
                currentTask.nextExecTime <= currentTime) {
            if (PQPop(&scheduler->tasks, &currentTask)) {
                TaskExecute(&currentTask);
                // Re-schedule task by updating its next execution time
                currentTask.nextExecTime = currentTime + (1000 / currentTask.hz);
                // Reinsert task with new priority
                PQPush(&scheduler->tasks, currentTask, currentTask.nextExecTime);
                executed = true;
            }
        }

        if (!executed) {
            // Sleep to avoid busy-waiting
            #ifdef _WIN32
            Sleep(sleepTime);
            #else
            struct timespec req, rem;
            req.tv_sec = sleepTime / 1000;
            req.tv_nsec = (sleepTime % 1000) * 1000000L;
            nanosleep(&req, &rem);
            #endif
        }
    }
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = 0;
}
