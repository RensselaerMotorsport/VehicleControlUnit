#ifndef RENSSELAERMOTORSPORT_ESCINDICATOR_H
#define RENSSELAERMOTORSPORT_ESCINDICATOR_H

#include "../DigitalSensor.h"
#include "stdbool.h"

typedef struct {
    DigitalSensor base;
    bool alive;
} ESCIndicator;

/*
 * @brief Initialization function for a Electronic stability control indicator sensor.
 *
 * @param esc A pointer to the ESCIndicator structure.
 * @pram hz Rate at which the sensor is called (in hz)
 * @pram port Location of sensor
 */
void initECSIndicator(ESCIndicator* esc, int hz, int port);

/**
 * TODO: Update this doc when function implemented.
 *
 * @brief Updated for this sensor.
 *
 * @param esc A pointer to the ESCIndicator structure.
 */
void updateECSInicator(void* esc);

/**
 * @brief Getter for alive var
 *
 * @param esc A pointer to the ESCIndicator structure.
 * @return If the sensor is alive (on)
 */
bool getAlive(ESCIndicator* esc);

#endif // RENSSELAERMOTORSPORT_ESCINDICATOR_H
