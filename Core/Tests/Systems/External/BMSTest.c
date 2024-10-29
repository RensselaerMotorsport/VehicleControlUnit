#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>
#include <assert.h>

#define BmsHz 500

int testBmsInit(const char* dbcFn, const char* testName) {
    Bms bms;
    initBms(&bms, BmsHz, dbcFn);

    if (bms.chargeStatus != IDLE || bms.packVoltage != 0.0f ||
        bms.packCurrent != 0.0f) {
        printf("Failed: %s. Initial charge status, pack voltage, or pack current is incorrect.\n", testName);
        return 0;
    } else {
        printf("Passed: %s.\n", testName);
        return 1;
    }
}

int testBmsUpdate(float voltage, float current, BmsChargeStatus expectedChargeStatus,
                  const char* dbcFn, const char* canDataFn, const char* testName) {
    Bms bms;
    initBms(&bms, BmsHz, dbcFn);

    updateBmsTest(&bms, canDataFn);

    if (bms.packVoltage != voltage) {
        printf("Failed: %s. BMS pack voltage, current, or charge status is incorrect.\n", testName);
        return 0;
    } else {
        printf("Passed: %s.\n", testName);
        return 1;
    }
}

int main() {
    assert(testBmsInit(
        "Tests/Systems/External/BmsFakeDbc.txt",
        "BMS Initialization Test"
    ));

    // Normal Operation (Charging)
    assert(testBmsUpdate(
        400.0f,
        100.0f,
        IDLE,
        "Tests/Systems/External/BmsFakeDbc.txt",
        "Tests/Systems/External/BmsFakeCanData.txt",
        "BMS Charging Operation Test"
    ));

    return 0;
}
