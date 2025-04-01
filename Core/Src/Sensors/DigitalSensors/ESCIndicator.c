#include "../../../Inc/Sensors/DigitalSensors/ESCIndicator.h"

void initECSIndicator(ESCIndicator* esc, int hz, int port) {
    initDigitalSensor(&esc->base, "ESC Indicator", hz, port, esc);
    esc->alive = true;
}

bool getAlive(ESCIndicator* esc) {
    return esc->alive;
}
