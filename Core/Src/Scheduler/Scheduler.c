#include <time.h>
#include <stdint.h>
#include "../../Inc/Scheduler/Scheduler.h"
// FIXME: Was supposed to be timer include.
// #include "../../Inc/stm32f7xx_hal_conf.h"

// Timer flag declared in the interrupt handler
// extern volatile uint32_t timer_flag;

bool SchedulerInit(Scheduler* scheduler, Updateable* updateables[],
                   int numUpdatables) {
    if (numUpdatables > MAX_UPDATEABLES) {
        // TODO: Make red when print helpers is added
        printf("Error: Number of sensors exceeds MAX_SENSORS. Some sensors will not be scheduled.\n");
        return false;
    }

    PQInit(&scheduler->tasks);
    scheduler->running = false;

    for (int i = 0; i < numUpdatables; i++) {
        Task task;

        Updateable* updateable = (Updateable*) updateables[i];
        if (updateable == nullptr) {
            // TODO: Make red when print helpers is added
            printf("Error: An Updatable is a nullptr\n");
            return false;
        }
        TaskInit(&task, updateable, updateable->hz);
        int hz = 1000 / updateable->hz;
        int initialPriority = clock() / (CLOCKS_PER_SEC / 1000) + hz;
        PQPush(&scheduler->tasks, task, initialPriority);
    }
    return true;
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
        // if (timer_flag < 0) continue;
        // timer_flag--;
        // FIXME: Re-implement timer.
        // int currentTime = HAL_GetTick();
        int currentTime = 0;

        while (!PQIsEmpty(&scheduler->tasks) &&
               PQPeek(&scheduler->tasks, &currentTask) &&
               currentTask.nextExecTime <= currentTime) {
            if (PQPop(&scheduler->tasks, &currentTask)) {
                TaskExecute(&currentTask);
                currentTask.nextExecTime = currentTime + (1000 / currentTask.hz);
                // Reinsert task with new priority
                PQPush(&scheduler->tasks, currentTask, currentTask.nextExecTime);
            }
        }
    }
}

void SchedulerStop(Scheduler* scheduler) {
    scheduler->running = 0;
}
