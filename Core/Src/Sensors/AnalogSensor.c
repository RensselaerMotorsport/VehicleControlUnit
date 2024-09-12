#include "../../Inc/Sensors/AnalogSensor.h"

void initAnalogSensor(AnalogSensor* analogsensor, const char* name, int hz, int channel) {
    initSensor(&analogsensor->sensor, name, hz, ANALOG);
    analogsensor->channel = channel;
}

