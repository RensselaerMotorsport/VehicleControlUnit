#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include <stdint.h>

#include "../ExternalSystem.h"
#include "../../../Inc/Systems/External/Can/DBCParser.h"
#include "BmsData.h"

typedef enum {
    DISCHARGING,
    CHARGING,
    IDLE,
} BmsChargeStatus;

typedef enum {
    Invalid_Signal = -1,
    BMS_PACK_VOLTAGE,
    BMS_PACK_CURRENT,
    BMS_STATE_OF_CHARGE,
    BMS_CELL_VOLTAGE_MIN,
    BMS_CELL_VOLTAGE_MAX,
    BMS_CELL_TEMPERATURE_MIN,
    BMS_CELL_TEMPERATURE_MAX,
    BMS_TOTAL_PACK_CAPACITY,
    BMS_REMAINING_PACK_CAPACITY,
    BMS_PACK_HEALTH,
    BMS_CHARGE_STATUS,
} BmsSignal;

typedef struct {
    unsigned int id;
    BmsSignal signalType;
} BmsSignalMap;

static BmsSignalMap bmsSignalMap[] = {
    {100, BMS_PACK_VOLTAGE},
    {200, BMS_PACK_CURRENT},
    {300, BMS_STATE_OF_CHARGE},
    {400, BMS_CELL_VOLTAGE_MIN},
    {500, BMS_CELL_VOLTAGE_MAX},
    {600, BMS_CELL_TEMPERATURE_MIN},
    {700, BMS_CELL_TEMPERATURE_MAX},
    {800, BMS_TOTAL_PACK_CAPACITY},
    {900, BMS_REMAINING_PACK_CAPACITY},
    {1000, BMS_PACK_HEALTH},
    {1100, BMS_CHARGE_STATUS},
};

typedef struct {
    ExternalSystem extSystem;
    DBC* dbc;
    Message** dbcMessageMap;

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

/** // FIXME: Modify this
 * @brief Transfers raw BMS data to a BmsData structure.
 *
 * @param rawData Pointer to the array of raw data.
 * @return The converted BmsData structure.
 */
BmsData transferFunctionBms(float* rawdata);

/**
 * @brief Updates the BMS data.
 *
 * @param bms Pointer to the Bms structure to update.
 */
void updateBms(void* bms);

/**
 * @brief Updates the BMS data.
 * @warning For testing and debugging use only
 *
 * @param bms Pointer to the Bms structure to update.
 * @param canDataFn Name of file containing can data.
 */
void updateBmsTest(void* bms, const char* canDataFn);

#endif // RENNSSELAERMOTORSPORT_BMS_H
