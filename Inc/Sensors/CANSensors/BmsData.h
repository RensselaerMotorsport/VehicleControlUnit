#ifndef RENNSSELAERMOTORSPORT_BMSDATA_H
#define RENNSSELAERMOTORSPORT_BMSDATA_H

#include <stdbool.h>

typedef struct {
    float chargeCurrentLimit;
    float dischargeCurrentLimit;
    float temperature;
    float stateOfCharge;
    float cellResistance;
    float packResistance;
    float cellVoltage;
    float packVoltage;
    bool criticalFaults;
} BmsData;

/**
 * @brief Initializes the BmsData structure with the given parameters.
 *
 * @param data Pointer to the BmsData structure to initialize.
 * @param ccl  Charge current limit.
 * @param dcl  Discharge current limit.
 * @param t    Temperature.
 * @param sc   State of charge.
 * @param cr   Cell resistance.
 * @param pr   Pack resistance.
 * @param cv   Cell voltage.
 * @param pv   Pack voltage.
 * @param cf   Critical faults flag.
 */
void initBmsData(BmsData* data, float ccl, float dcl, float t, float sc,
                 float cr, float pr, float cv, float pv, bool cf);

/**
 * @brief Converts the BmsData structure to a string.
 *
 * @param data Pointer to the BmsData structure to convert.
 * @return     Pointer to the string representation of the BmsData structure.
 */
char* toStringBmsData(const BmsData* data);

#endif // RENNSSELAERMOTORSPORT_BMSDATA_H
