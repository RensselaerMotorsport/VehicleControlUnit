#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

#define BmsHz 500

int testBmsInit(const char* dbcFn, const char* testName) {
    Bms bms;
    initBms(&bms, BmsHz, dbcFn);

    if (bms.chargeStatus != IDLE || bms.packVoltage != 0.0f ||
        bms.packCurrent != 0.0f) {
        printf("Failed: %s. Initial charge status, pack voltage, or pack "
               "current is incorrect.\n", testName);
        return 1;
    }

    printf("Passed: %s.\n", testName);
    return 0;
}

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

int testBmsUpdate(Bms* expectedBms, const char* dbcFn, const char* canFn,
                  const char* testName) {
    Bms bms;
    initBms(&bms, BmsHz, dbcFn);
    updateBmsTest(&bms, canFn);

    if (equal(&bms, expectedBms)) {
        printf("Failed: %s. BMS ...\n", testName);
        return 1;
    }

    printf("Passed: %s.\n", testName);
    return 0;
}

int bms_main() {
    int result = 0;

    result += testBmsInit(
        "Tests/Systems/External/Orion_CANBUS.dbc",
        "BMS Initialization Test"
    );

    Bms expectedBms;
    initBms(&expectedBms, BmsHz, "Tests/Systems/External/Orion_CANBUS.dbc");
    expectedBms.packVoltage = 600.0f;
    expectedBms.packVoltage = 400.0f;
    expectedBms.stateOfCharge = 80.0f;

    result += testBmsUpdate(
        &expectedBms,
        "Tests/Systems/External/Orion_CANBUS.dbc",
        "Tests/Systems/External/BmsFakeCanData.txt",
        "Bms Update Test: First Message (1712)"
    );

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return 0;
}
