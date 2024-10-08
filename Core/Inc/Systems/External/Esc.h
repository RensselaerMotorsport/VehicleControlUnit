#ifndef RENNSSELAERMOTORSPORT_ESC_H
#define RENNSSELAERMOTORSPORT_ESC_H

#include "../ExternalSystem.h"

typedef struct {
    // CANSensor base;
    float temp;
    float motor_revolutions;
} Esc;

/**
 * @brief Initializes the ESC with the given frequency.
 *
 * @param esc Pointer to the Esc structure to initialize.
 * @param hz  The frequency in Hertz at which the ESC operates.
 */
void initEsc(Esc* esc, int hz);

/**
 * @brief Gets the temperature of the ESC.
 *
 * @param esc Pointer to the Esc structure to query.
 * @return    The temperature of the ESC.
 */
float getEscTemp(const Esc* esc);

/**
 * @brief Gets the motor revolutions of the ESC.
 *
 * @param esc Pointer to the Esc structure to query.
 * @return    The motor revolutions of the ESC.
 */
float getEscMotorRevolutions(const Esc* esc);

/**
 * @brief Updates the ESC data.
 *
 * @param esc Pointer to the Esc structure to update.
 */
void updateEsc(void* esc);

/**
 * @brief Converts the Esc structure to a string.
 *
 * @param esc Pointer to the Esc structure to convert.
 * @return    Pointer to the string representation of the Esc structure.
 */
char* toStringEsc(const Esc* esc);

#endif // RENNSSELAERMOTORSPORT_ESC_H
