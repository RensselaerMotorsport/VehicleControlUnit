/**
  ******************************************************************************
  * @file           : SchedulerTest.c
  * @brief          : Unit tests for the scheduler.
  ******************************************************************************
*/

#include "../Inc/Scheduler/Scheduler.h"
#include "../Inc/Systems/Controller/Apps.h"
#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"

int testSchedulerInit() {
    Scheduler scheduler;
    int numSensors = 2;

    // Initialize Updateables
    Apps apps;
    int appsHz = 200;
    initApps(&apps, appsHz, 0, 1);

    WheelSpeed wheelSpeed;
    int wheelSpeedHz = 200;
    float radius = 0.1f;
    int numTeeth = 40;
    initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

    void* updatableArr[numSensors];
    updatableArr[0] = &apps;
    updatableArr[1] = &wheelSpeed;

    if (SchedulerInit(&scheduler, &updatableArray, numSensors))
        return 0;
    return 1;
}

int main() {
    int result = 0;

    result += testSchedulerInit();

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
