#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include "../CANSensor.h"
#include "BmsData.h"

typedef struct {
    CANSensor base;
    BmsData data;
} Bms;

/**
 * @brief Initializes the BMS with the given frequency.
 *
 * @param bms Pointer to the Bms structure to initialize.
 * @param hz  The frequency in Hertz at which the BMS operates.
 */
void initBms(Bms* bms, int hz);

/**
 * @brief Updates the BMS data.
 *
 * @param bms Pointer to the Bms structure to update.
 */
void updateBms(void* bms);

/**
 * @brief Transfers raw BMS data to a BmsData structure.
 *
 * @param rawData Pointer to the array of raw data.
 * @return The converted BmsData structure.
 */
BmsData transferFunctionBms(float* rawdata);

#endif // RENNSSELAERMOTORSPORT_BMS_H
