#include "../Inc/Scheduler/Scheduler.h"
#include "../Inc/Sensors/AnalogSensors/App.h"
#include "../Inc/Sensors/AnalogSensors/Temperature.h"
#include "../Inc/Sensors/AnalogSensors/ShockPot.h"

int main() {
    Scheduler scheduler;
    Sensor* sensorArray[MAX_SENSORS];

    App appSensor;
    Temperature temperatureSensor;
    ShockPot shockPotSensor;

    initApp(&appSensor, 200, 1);
    initTemperature(&temperatureSensor, 10, 2);
    initShockPot(&shockPotSensor, 1000, 3, FRONT_LEFT);

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
