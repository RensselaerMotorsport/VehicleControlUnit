#ifndef RENNSSELAERMOTORSPORT_BMS_H
#define RENNSSELAERMOTORSPORT_BMS_H

#include <stdint.h>

#include "../ExternalSystem.h"
#include "BmsData.h"

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
    const char* signalName;
    BmsSignal signalType;
} BmsSignalMap;

static BmsSignalMap bmsSignalMap[] = {
    {"Pack_Voltage", BMS_PACK_VOLTAGE},
    {"Pack_Current", BMS_PACK_CURRENT},
    {"State_of_Charge", BMS_STATE_OF_CHARGE},
    {"Cell_Voltage_Min", BMS_CELL_VOLTAGE_MIN},
    {"Cell_Voltage_Max", BMS_CELL_VOLTAGE_MAX},
    {"Cell_Temperature_Min", BMS_CELL_TEMPERATURE_MIN},
    {"Cell_Temperature_Max", BMS_CELL_TEMPERATURE_MAX},
    {"Total_Pack_Capacity", BMS_TOTAL_PACK_CAPACITY},
    {"Remaining_Pack_Capacity", BMS_REMAINING_PACK_CAPACITY},
    {"Pack_Health", BMS_PACK_HEALTH},
    {"Charge_Status", BMS_CHARGE_STATUS},
};

typedef struct {
    ExternalSystem extSystem;

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
    uint8_t chargeStatus; // 0: Discharging, 1: Charging, 2: Idle
} Bms;

/**
 * @brief Initializes the BMS with the given frequency.
 *
 * @param bms Pointer to the Bms structure to initialize.
 * @param hz  The frequency in Hertz at which the BMS operates.
 */
void initBms(Bms* bms, int hz);

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
 * @param dbcFilename Name of file containing dbc.
 */
void updateBmsTest(void* bms, const char* dbcFilename);

#endif // RENNSSELAERMOTORSPORT_BMS_H
