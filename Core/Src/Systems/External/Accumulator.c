#include "../../../Inc/Systems/External/Accumulator.h"

void initAccumulator(Accumulator* acc, Bms* bms) {
    if (acc != NULL) {
        acc->bms = bms;
        acc->state = standby;  // Default state
    }
}

void setAccumulatorState(Accumulator* acc, CHARGE_STATE state) {
    if (acc != NULL) {
        acc->state = state;
    }
}

CHARGE_STATE getAccumulatorState(const Accumulator* acc) {
    if (acc != NULL) {
        return acc->state;
    }
    return standby;  // Default if not initialized
}
