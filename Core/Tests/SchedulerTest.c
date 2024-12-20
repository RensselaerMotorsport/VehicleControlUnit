/**
  ******************************************************************************
  * @file           : SchedulerTest.c
  * @brief          : Unit tests for the scheduler.
  ******************************************************************************
*/

#include "../Inc/Scheduler/Scheduler.h"
#include "../Inc/Sensors/AnalogSensors/App.h"
#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"

#define NUM_SENSORS 2

Updateable** initUpdatables() {
    static WheelSpeed wheelSpeed;
    int wheelSpeedHz = 1; // Small hz so its readable
    float radius = 0.1f;
    int numTeeth = 40;
    initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

    static App app;
    int appHz = 2;
    initApp(&app, appHz, 0);

    static Updateable* updateableArr[NUM_SENSORS + 1];

    updateableArr[0] = GetUpdateableWheelSpeed(&wheelSpeed);
    updateableArr[1] = GetUpdateableApp(&app);
    updateableArr[NUM_SENSORS] = NULL;

    return updateableArr;
}

int testSchedulerInit() {
    const char* testName = "SchedulerInit Test";
    Scheduler scheduler;

    // Initialize Updateables
    Updateable** updateableArr = initUpdatables();

    // Initialize Scheduler
    if (!SchedulerInit(&scheduler, updateableArr)) { // Expect true
        printf("%s failed.\nExpected: %d\nGot: %d\n", testName, 1, 0);
        return 1;
    }

    // Validate priority queue content
    Task task;
    if (!PQPeek(&scheduler.tasks, &task)) { // Ensure the queue is not empty
        printf("%s failed.\nExpected non-empty priority queue.\n", testName);
        return 1;
    }

    Updateable* expectedUpdateable = updateableArr[0];
    if (task.updateable != expectedUpdateable) {
        printf("%s failed.\nExpected Updateable Address: %p\nGot: %p\n",
               testName, (void*)expectedUpdateable, (void*)task.updateable);
        return 1;
    }

    printf("%s passed.\n", testName);
    return 0;
}

int testSchedulerRunLimited() {
    // Init Scheduler
    Scheduler scheduler;
    Updateable** updateableArr = initUpdatables();
    SchedulerInit(&scheduler, updateableArr);

    // Scheduler Test
    SchedulerRun(&scheduler);

    return 0;
}

int main() {
    printf("Starting Scheduler Test...\n");
    int result = 0;

    result += testSchedulerInit();
    result += testSchedulerRunLimited();

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
