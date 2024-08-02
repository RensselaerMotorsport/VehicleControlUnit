#include "../../Inc/Utils/Conversions.h"

static const float MPS_TO_MPH = 2.236936;

float convertMpsToMph(float metersPerSecond) {
    return metersPerSecond * MPS_TO_MPH;
}
