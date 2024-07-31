#include "../../Inc/Sensors/DigitalSensor.h"

void initDigitalSensor(DigitalSensor* digitalSensor, const char* name, int hz,
                       int port) {
    initSensor(&digitalSensor->sensor, name, hz, DIGITAL);
    digitalSensor->port = port;
}
