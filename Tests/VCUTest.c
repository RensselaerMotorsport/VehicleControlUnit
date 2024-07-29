#include <stdio.h>
#include "../includes/Scheduler.h"
#include "../includes/App.h"
#include "../includes/Temperature.h"
#include "../includes/ShockPot.h"

int main() {
    Scheduler scheduler;
    Sensor* sensorArray[MAX_SENSORS];

    App appSensor;
    Temperature temperatureSensor;
    ShockPot shockPotSensor;

    initApp(&appSensor, 200, 1);
    initTemperature(&temperatureSensor, 10, 2);
    initShockPot(&shockPotSensor, 1000, 3, front_left);

    sensorArray[0] = (Sensor*)&appSensor;
    sensorArray[1] = (Sensor*)&temperatureSensor;
    sensorArray[2] = (Sensor*)&shockPotSensor;

    // Initialize Scheduler with sensors
    SchedulerInit(&scheduler, sensorArray, 3);

    // Run Scheduler to process tasks
    SchedulerRun(&scheduler);

    // Figure out stopping mechanism. For now ctrl-c
    // SchedulerStop(&scheduler);
    // printf("Scheduler has stopped running.\n");

    return 0;
}
