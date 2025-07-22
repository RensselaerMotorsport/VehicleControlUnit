#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include <stdint.h>

#include "../ExternalSystem.h"
#include "../Inc/Systems/Comms/Can/DBCParser.h"

typedef enum {
    DISCHARGING,
    CHARGING,
    IDLE,
} BmsChargeStatus;

typedef struct {
    ExternalSystem extSystem;

    // Bms Data
    float packVoltage;
    float packCurrent;
    float stateOfCharge;
    float cellVoltageMin;
    float cellVoltageMax;
    float cellTemperatureMin;
    float cellTemperatureMax;
    float totalPackCapacity;
    float remainingPackCapacity;
    float packHealth;
    BmsChargeStatus chargeStatus;
} Bms;

/**
 * @brief Initializes the BMS with the given frequency.
 *
 * @param bms Pointer to the Bms structure to initialize.
 * @param hz  The frequency in Hertz at which the BMS operates.
 * @param dbcFn The filename of the dbc file to use to interpret the can data.
 */
void initBms(Bms* bms, int hz, const char* dbcFn);

/**
 * @brief Transfers raw BMS data to a BmsData structure.
 *
 * @param rawData Pointer to the array of raw data.
 * @return The converted BmsData structure.
 */
//BmsData transferFunctionBms(float* rawdata);

/**
 * @brief Updates the BMS data.
 *
 * @param updateable Pointer to the Updateable structure to update.
 * @return int _SUCCESS or _FAILURE.
 */
int updateBms(Updateable* updateable);

/**
 * @brief Updates the BMS data.
 * @warning For testing and debugging use only
 *
 * @param bms Pointer to the Bms structure to update.
 * @param canDataFn Name of file containing can data.
 */
void updateBmsTest(void* bms, const char* canDataFn);

#endif // RENNSSELAERMOTORSPORT_BMS_H
