#include <stdint.h>

#include "../../Inc/Scheduler/Scheduler.h"
#include "../../Inc/Systems/PrintHelpers.h"
#include "../../Inc/Utils/PlatformTime.h"

/**
 * @brief Added Updateable to the schedulers priority queue.
 *
 * @param sched Scheduler thats being added to.
 * @param item The Updateable to add.
 * @return True if added correctly; false otherwise.
 */
bool addUpdatableToScheduler(Scheduler* sched, const Updateable* item) {
    if (item->hz <= 0 || item->hz > MAX_HZ) {
        printf(ANSI_COLOR_RED
            "Error: Updatable hz is outside of normal range. hz - %d\n"
            ANSI_COLOR_RESET, item->hz
        );
        return false;
    }

    Task task;
    TaskInit(&task, item, item->hz);
    int hz = 1000 / item->hz;
    int initialPriority = CURRENT_TIME_MS() + hz;
    PQPush(&sched->tasks, task, initialPriority);
    return true;
}

/**
 * Initializes the priority queue and schedules tasks based on the given sensors
 * and their update frequencies. Limits the number of sensors to MAX_SENSORS.
 */
bool SchedulerInit(Scheduler* scheduler, Updateable* updatableArray[]) {
    PQInit(&scheduler->tasks);
    scheduler->running = false;

    for (int i = 0; updatableArray[i] != NULL; i++) {
        if (i >= MAX_UPDATEABLES) {
            printf(ANSI_COLOR_RED
                "Error: Number of sensors exceeds MAX_SENSORS.\n"
                ANSI_COLOR_RESET
            );
            return false;
        }

        if (!addUpdatableToScheduler(scheduler, updatableArray[i]))
            return false;
    }
    return true;
}

/**
 * Check if there are tasks due to run
 *
 * @param scheduler Pointer to scheduler structure
 * @param currentTime The time in which were processing
 * @return True if there is task ready. False otherwise.
 */
bool HasDueTask(Scheduler* scheduler, int currentTime) {
    Task peekTask;
    return !PQIsEmpty(&scheduler->tasks) &&
           PQPeek(&scheduler->tasks, &peekTask) &&
           peekTask.nextExecTime <= currentTime;
}

/**
 * Processes each task for a given time
 *
 * @param scheduler Pointer to scheduler structure
 * @param currentTime The time in which were processing
 */
void ProcessDueTasks(Scheduler* scheduler, int currentTime) {
    Task currentTask;
    while (HasDueTask(scheduler, currentTime)) {
        // Fetch next task
        if (!PQPop(&scheduler->tasks, &currentTask)) {
            continue;
        }
        // Execute task
        TaskExecute(&currentTask);
        printf("Updating: %s.\n", currentTask.updateable->name);
        currentTask.nextExecTime = currentTime + (1000 / currentTask.hz);
        // Reschedule task with new priority
        PQPush(&scheduler->tasks, currentTask, currentTask.nextExecTime);
    }
}

void SchedulerRun(Scheduler* scheduler) {
    scheduler->running = true;

    while (scheduler->running) {
        int currentTime = CURRENT_TIME_MS();
        ProcessDueTasks(scheduler, currentTime);
    }
}

/// @warning For testing proposes only
void SchedulerRunLimited(Scheduler* scheduler, int numItterations) {
    scheduler->running = true;

    for (int i = 0; i < numItterations; ++i) {
        int currentTime = CURRENT_TIME_MS();
        ProcessDueTasks(scheduler, currentTime);
    }

    scheduler->running = false;
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = false;
}
