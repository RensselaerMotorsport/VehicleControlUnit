#include "../../Inc/Sensors/DigitalSensor.h"

void initDigitalSensor(DigitalSensor* digitalSensor, const char* name, int hz,
                       int port) {
    initSensor(&digitalSensor->sensor, name, hz, s_DIGITAL);
    digitalSensor->port = port;
}

int getDigitalSensorReading(DigitalSensor* digitalSensor) {
    return digital_in_buffer[digitalSensor->port];
}