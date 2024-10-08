#include "../../../Inc/Systems/External/BMS.h"

#include <stdio.h>
#include <assert.h>

#define BmsHz 500

int testBmsInit(const char* testName) {
    Bms bms;
    initBms(&bms, BmsHz);

    if (bms.chargeStatus != IDLE || bms.packVoltage != 0.0f ||
        bms.packCurrent != 0.0f) {
        printf("Failed: %s. Initial charge status, pack voltage, or pack current is incorrect.\n", testName);
        return 0;
    } else {
        printf("Passed: %s.\n", testName);
        return 1;
    }
}

// int testBmsUpdate(float voltage, float current, ChargeStatus expectedChargeStatus,
//                   const char* dbcFile, const char* testName) {
//     Bms bms;
//     initBms(&bms, BmsHz);
//
//     updateBmsTest(&bms, dbcFile);
//
//     if (getBmsPackVoltage(&bms) != voltage || getBmsPackCurrent(&bms) != current ||
//         getBmsChargeStatus(&bms) != expectedChargeStatus) {
//         printf("Failed: %s. BMS pack voltage, current, or charge status is incorrect.\n", testName);
//         return 0;
//     } else {
//         printf("Passed: %s.\n", testName);
//         return 1;
//     }
// }
//
// int testBmsResetStatus(const char* testName) {
//     Bms bms;
//     initBms(&bms, BmsHz);
//
//     setBmsChargeStatus(&bms, CHARGE_STATUS_CHARGING);
//     resetBmsStatus(&bms);
//
//     if (getBmsChargeStatus(&bms) != CHARGE_STATUS_IDLE) {
//         printf("Failed: %s. Charge status was not reset correctly.\n", testName);
//         return 0;
//     } else {
//         printf("Passed: %s.\n", testName);
//         return 1;
//     }
// }

int main() {
    assert(testBmsInit("BMS Initialization Test"));

    // // Normal Operation (Charging)
    // assert(testBmsUpdate(400.0f, -50.0f, CHARGE_STATUS_CHARGING, "BMS Charging Operation Test"));
    // // Normal Operation (Discharging)
    // assert(testBmsUpdate(400.0f, 50.0f, CHARGE_STATUS_DISCHARGING, "BMS Discharging Operation Test"));
    // // Overvoltage Condition
    // assert(testBmsUpdate(500.0f, 0.0f, CHARGE_STATUS_FAULT, "BMS Overvoltage Condition Test"));
    // // Undervoltage Condition
    // assert(testBmsUpdate(200.0f, 0.0f, CHARGE_STATUS_FAULT, "BMS Undervoltage Condition Test"));
    // // Overcurrent Condition
    // assert(testBmsUpdate(400.0f, 200.0f, CHARGE_STATUS_FAULT, "BMS Overcurrent Condition Test"));
    //
    // // Resetting BMS status
    // assert(testBmsResetStatus("BMS Reset Status Test"));

    return 0;
}
