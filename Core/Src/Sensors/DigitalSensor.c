#include "../../Inc/Sensors/DigitalSensor.h"

void initDigitalSensor(DigitalSensor* digitalSensor, const char* name, int hz,
                       int port, void* child) {
    initSensor(&digitalSensor->sensor, name, hz, s_DIGITAL, digitalSensor);
    digitalSensor->port = port;
    digitalSensor->child = child;
}

int getDigitalSensorReading(DigitalSensor* digitalSensor) {
    return digital_in_buffer[digitalSensor->port];
}