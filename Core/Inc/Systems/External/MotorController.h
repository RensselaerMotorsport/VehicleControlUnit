#ifndef RENNSSELAERMOTORSPORT_MOTORCONTROLLER_H
#define RENNSSELAERMOTORSPORT_MOTORCONTROLLER_H

#include "Esc.h"
#include "../../Sensors/AnalogSensors/App.h"

typedef struct {
    Esc* esc;
    App* app;
} MotorController;

/**
 * @brief Initializes the MotorController with the given ESC and APP.
 *
 * @param mc  Pointer to the MotorController structure to initialize.
 * @param esc Pointer to the Esc structure.
 * @param app Pointer to the App structure.
 */
void initMotorController(MotorController* mc, Esc* esc, App* app);

/**
 * @brief Gets the pedal position from the MotorController.
 *
 * @param mc Pointer to the MotorController structure to query.
 * @return   The pedal position, or 0.0 if no APP is available.
 */
float getPedalPosition(const MotorController* mc);

/**
 * @brief Gets the motor temperature from the MotorController.
 *
 * @param mc Pointer to the MotorController structure to query.
 * @return   The motor temperature, or 0.0 if no ESC is available.
 */
float getMotorTemp(const MotorController* mc);

#endif // RENNSSELAERMOTORSPORT_MOTORCONTROLLER_H
