#include "../Inc/Systems/Controller/Apps.h"

#include "stdio.h"

int testAppInit(const char* testName) {
    Apps apps;
    initApps($apps, 0, 0, 1);

    if (apps.status != APPS_OK) {
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 1;
    } else {
        printf("%s Passed: Initial status is OK.\n", testName);
        return 0;
    }
}

int testAppsUpdate(float pos1, float pos2, AppStatus desiredStatus, const char* testName) {
    Apps apps;
    initApps($apps, 0, 0, 1);

    setAppPos(&apps.app[0], pos1);
    setAppPos(&apps.app[1], pos1);
    updateApps(&apps);

    if (apps.status != desiredStatus) {
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 1;
    } else {
        printf("%s Passed: Initial status is OK.\n", testName);
        return 0;
    }
}

int main() {
    int result = 0;

    result += testAppsInit("Initialization Test");
    // Normal Operation
    result += testAppsUpdate(0, 0, APPS_OK, "Normal Operation at 0%");
    result += testAppsUpdate(50, 50, APPS_OK, "Normal Operation at 50%");
    result += testAppsUpdate(100, 100, APPS_OK, "Normal Operation at 100%");

    // Edge of Acceptable Difference (10%)
    result += testAppsUpdate(0, 10, APPS_OK, "Max Edge Difference at 10%");
    result += testAppsUpdate(55, 60, APPS_OK, "Edge Difference at 10%");
    result += testAppsUpdate(99, 100, APPS_OK, "Edge Difference at 10%");

    // Implausibility Due to Difference >10%
    result += testAppsUpdate(0, 10.1, APPS_FAULT, "Min Implausibility at 11%");
    result += testAppsUpdate(50, 61, APPS_FAULT, "Implausibility at 11%");
    result += testAppsUpdate(0, 100, APPS_FAULT, "Implausibility at 100%");

    // Sensor Failure (Out of Range)
    result += testAppsUpdate(-1, 50, APPS_FAULT, "Sensor Failure: pos1 out of range (-1%)");
    result += testAppsUpdate(50, -1, APPS_FAULT, "Sensor Failure: pos2 out of range (-1%)");
    result += testAppsUpdate(101, 50, APPS_FAULT, "Sensor Failure: pos1 out of range (101%)");
    result += testAppsUpdate(50, 101, APPS_FAULT, "Sensor Failure: pos2 out of range (101%)");

    // Both Sensors Failed
    result += testAppsUpdate(-1, -1, APPS_FAULT, "Both Sensors Failed (-1%)");
    result += testAppsUpdate(101, 101, APPS_FAULT, "Both Sensors Failed (101%)");

    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}