#ifndef RENNSSELAERMOTORSPORT_ACCUMULATOR_H
#define RENNSSELAERMOTORSPORT_ACCUMULATOR_H

#include "Bms.h"
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

void initAccumulator(Accumulator* acc, Bms* bms, Imd* imd);
void setAccumulatorState(Accumulator* acc, CHARGE_STATE state);
CHARGE_STATE getAccumulatorState(const Accumulator* acc);

#endif
