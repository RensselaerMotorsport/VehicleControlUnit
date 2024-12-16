/**
  ******************************************************************************
  * @file           : SchedulerTest.c
  * @brief          : Unit tests for the scheduler.
  ******************************************************************************
*/

#include "../Inc/Scheduler/Scheduler.h"
#include "../Inc/Systems/Controller/Apps.h"
#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"

Updateable** initUpdatables() {
    int numSensors = 2;

    Apps apps;
    int appsHz = 200;
    initApps(&apps, appsHz, 0, 1);

    WheelSpeed wheelSpeed;
    int wheelSpeedHz = 200;
    float radius = 0.1f;
    int numTeeth = 40;
    initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

    // Dynamically allocate the array
    Updateable** updateableArr = malloc((numSensors + 1) * sizeof(Updateable*));
    if (!updateableArr) return NULL;

    updateableArr[0] = GetUpdateableApps(&apps);
    updateableArr[1] = &(wheelSpeed.base.sensor.updateable);
    updateableArr[2] = NULL;
    return updateableArr;
}

int testSchedulerInit() {
    Scheduler scheduler;

    // Initialize Updateables
    Updateable** updateableArr = initUpdatables();

    // Initialize Scheduler
    if (SchedulerInit(&scheduler, updateableArr))
        return 0;
    return 1;
}

int testSchedulerRun() {
    // Init Scheduler
    Scheduler scheduler;
    Updateable** updateableArr = initUpdatables();
    SchedulerInit(&scheduler, updateableArr);

    // Scheduler Test
    SchedulerRun(&scheduler);

    return 1;
}

int main() {
    printf("Starting Scheduler Test...\n");
    int result = 0;

    result += testSchedulerInit();
    result += testSchedulerRun();

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
