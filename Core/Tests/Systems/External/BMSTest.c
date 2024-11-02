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
        return 1;
    } else {
        printf("Passed: %s.\n", testName);
        return 0;
    }
}

int main() {
    int result = 0;

    result += testBmsInit(
        "Tests/Systems/External/BmsFakeDbc.txt",
        "BMS Initialization Test"
    );

    // Display overall test result
    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    // TODO: Add update test suit

    return 0;
}
