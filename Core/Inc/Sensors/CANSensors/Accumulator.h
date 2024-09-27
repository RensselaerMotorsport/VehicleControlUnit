#ifndef RENNSSELAERMOTORSPORT_ACCUMULATOR_H
#define RENNSSELAERMOTORSPORT_ACCUMULATOR_H

#include "BMS.h"
#include "Imd.h"

typedef enum {
    charge,
    discharge,
    standby
} CHARGE_STATE;

typedef struct {
    Bms* bms;
    Imd* imd;
    CHARGE_STATE state;
} Accumulator;

/**
 * @brief Initializes the accumulator with the given BMS and IMD.
 *
 * @param acc Pointer to the Accumulator structure to initialize.
 * @param bms Pointer to the Bms structure.
 * @param imd Pointer to the Imd structure.
 */
void initAccumulator(Accumulator* acc, Bms* bms, Imd* imd);

/**
 * @brief Sets the state of the accumulator.
 *
 * @param acc   Pointer to the Accumulator structure to update.
 * @param state The desired charge state to set.
 */
void setAccumulatorState(Accumulator* acc, CHARGE_STATE state);

/**
 * @brief Gets the current state of the accumulator.
 *
 * @param acc Pointer to the Accumulator structure to query.
 * @return    The current charge state of the accumulator, or standby if not initialized.
 */
CHARGE_STATE getAccumulatorState(const Accumulator* acc);

#endif // RENNSSELAERMOTORSPORT_ACCUMULATOR_H
