#include "../../../Inc/Systems/External/BMS.h"
#include "test.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>

// TODO: Put in bms file
#define FLOAT_ERROR 1e-5
bool floatEqual(float a, float b) {
    return fabs(a - b) < FLOAT_ERROR;
}

bool equal(Bms* bms1, Bms* bms2) {
    if (!floatEqual(bms1->packVoltage, bms2->packVoltage)) return false;
    if (!floatEqual(bms1->packCurrent, bms2->packCurrent)) return false;
    if (!floatEqual(bms1->stateOfCharge, bms2->stateOfCharge)) return false;
    if (!floatEqual(bms1->cellVoltageMin, bms2->cellVoltageMin)) return false;
    if (!floatEqual(bms1->cellVoltageMax, bms2->cellVoltageMax)) return false;
    if (!floatEqual(bms1->cellTemperatureMin, bms2->cellTemperatureMin)) return false;
    if (!floatEqual(bms1->cellTemperatureMax, bms2->cellTemperatureMax)) return false;
    if (!floatEqual(bms1->totalPackCapacity, bms2->totalPackCapacity)) return false;
    if (!floatEqual(bms1->remainingPackCapacity, bms2->remainingPackCapacity)) return false;
    if (!floatEqual(bms1->packHealth, bms2->packHealth)) return false;
    // TODO: Add charge status

    return true;
}

#define BmsHz 500

void bms_main() {
    TEST(bms_init, {
      Bms bms;
      initBms(&bms, BmsHz, "Tests/Systems/External/Orion_CANBUS.dbc");
      ASSERT(bms.chargeStatus == IDLE, "bms is idle", "bms is not idle");
      ASSERT_EQ(bms.packVoltage, 0, "pack voltage", "expected pack voltage");
      ASSERT_EQ(bms.packCurrent, 0, "pack current", "expected pack current");
    })

    Bms expectedBms;
    initBms(&expectedBms, BmsHz, "Tests/Systems/External/Orion_CANBUS.dbc");
    expectedBms.packVoltage = 600.0f;
    expectedBms.packVoltage = 400.0f;
    expectedBms.stateOfCharge = 80.0f;

    TEST(bms_update, {
      Bms bms;
      initBms(&bms, BmsHz, "Tests/Systems/External/Orion_CANBUS.dbc");
      updateBmsTest(&bms, "Tests/Systems/External/BmsFakeCanData.txt");
      // FIXME Should this be equal or !equal? Original had it as equal is a failure
      ASSERT(!equal(&bms, &expectedBms), "bms doesn't equal expected bms", "bms equals expected bms");
    })
}
