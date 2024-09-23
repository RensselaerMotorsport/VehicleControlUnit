#ifndef RENNSSELAERMOTORSPORT_IMD_H
#define RENNSSELAERMOTORSPORT_IMD_H

#include <stdbool.h>

#include "../CANSensor.h"

typedef struct {
    CANSensor base;
    bool imd_status;
    bool running_flag;
} Imd;

/**
 * @brief Initializes the IMD with the given frequency.
 *
 * @param imd Pointer to the Imd structure to initialize.
 * @param hz  The frequency in Hertz at which the IMD operates.
 */
void initImd(Imd* imd, int hz);

/**
 * @brief Gets the status of the IMD.
 *
 * @param imd Pointer to the Imd structure to query.
 * @return    The status of the IMD.
 */
bool getImdStatus(const Imd* imd);

/**
 * @brief Gets the running flag of the IMD.
 *
 * @param imd Pointer to the Imd structure to query.
 * @return    The running flag of the IMD.
 */
bool getRunningFlag(const Imd* imd);

/**
 * @brief Resets the running flag of the IMD.
 *
 * @param imd Pointer to the Imd structure to update.
 */
void resetRunningFlag(Imd* imd);

/**
 * @brief Updates the IMD data.
 *
 * @param imd Pointer to the Imd structure to update.
 */
void updateImd(void* imd);

/**
 * @brief Converts the Imd structure to a string.
 *
 * @param imd Pointer to the Imd structure to convert.
 * @return    Pointer to the string representation of the Imd structure.
 */
char* toStringImd(const Imd* imd);

#endif // RENNSSELAERMOTORSPORT_IMD_H
