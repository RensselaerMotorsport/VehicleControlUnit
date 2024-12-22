/**
  ******************************************************************************
  * @file           : SchedulerTest.c
  * @brief          : Unit tests for the scheduler.
  ******************************************************************************
*/

#include "../Inc/Scheduler/Scheduler.h"
#include "../Inc/Sensors/AnalogSensors/App.h"
#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"
#include "../Inc/Systems/PrintHelpers.h"

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
    if (!SchedulerInit(&scheduler, updateableArr)) {
        printf(ANSI_COLOR_RED "%s failed.\nExpected: %d\nGot: %d\n" ANSI_COLOR_RESET, testName, 1, 0);
        return 1;
    }

    // Validate priority queue content
    Task task;
    if (!PQPeek(&scheduler.tasks, &task)) { // Ensure the queue is not empty
        printf(ANSI_COLOR_RED "%s failed.\nExpected non-empty priority queue.\n" ANSI_COLOR_RESET, testName);
        return 1;
    }

    Updateable* expectedUpdateable = updateableArr[1];
    if (task.updateable != expectedUpdateable) {
        printf(ANSI_COLOR_RED "%s failed.\nExpected Updateable Address: %p\nGot: %p\n" ANSI_COLOR_RESET,
               testName, (void*)expectedUpdateable, (void*)task.updateable);
        return 1;
    }

    printf("%s passed.\n", testName);
    return 0;
}

int testSchedulerRunLimited() {
    const char* testName = "SchedulerRun Test";

    // Init Scheduler
    Scheduler scheduler;

    // Init Sensor
    static WheelSpeed wheelSpeed;
    int wheelSpeedHz = 2;
    float radius = 0.15f;
    int numTeeth = 40;
    initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

    // Add speed params
    addPulse(&wheelSpeed, 500);
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000;   // 1 Millisecond

    if (nanosleep(&ts, NULL) < 0) {
        perror("nanosleep failed");
        return 1;
    }

    // Create Updateable array
    static Updateable* updateableArr[2];
    updateableArr[0] = GetUpdateableWheelSpeed(&wheelSpeed);
    updateableArr[1] = NULL;

    SchedulerInit(&scheduler, updateableArr);

    // Scheduler Test
    ProcessDueTasks(&scheduler, 0);

    /// @note This test requires wheel speed test to also work.
    float speed = wheelSpeed.speed; 
    int expectedSpeedLow = 26;
    int expectedSpeedHigh = 27;
    if (speed < expectedSpeedLow && speed > expectedSpeedHigh) {
        printf(ANSI_COLOR_RED "%s failed.\nExpected: %d - %d\nGot: %f\n" ANSI_COLOR_RESET,
               testName, expectedSpeedLow, expectedSpeedHigh, speed);
        return 1;
    }

    printf("%s passed.\n", testName);
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
