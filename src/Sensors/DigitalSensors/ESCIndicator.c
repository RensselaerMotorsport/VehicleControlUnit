#include "../../../includes/ESCIndicator.h"

/*
 * Initialization function for a Electronic stability control indicator sensor.
 *
 * @param esc A pointer to the ESCIndicator structure.
 * @pram hz Rate at which the sensor is called (in hz)
 * @pram port Location of sensor
 */
void initWheelSpeed(ESCIndicator* esc, int hz, int port) {
    initDigitalSensor(&esc->base, "ESC Indicator", hz, port);
    esc->alive = true;
}

/*
 * Getter for alive var
 *
 * @param esc A pointer to the ESCIndicator structure.
 * @return If the sensor is alive (on)
 */
bool getAlive(ESCIndicator* esc) {
    return esc->alive;
}
