#include "../Inc/Systems/Controller/Apps.h"

#include "stdio.h"

int testAppsInit(const char* testName) {
    Apps apps;
    initApps(&apps, 0, 0, 1);

    if (apps.status != APPS_OK) {
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 1;
    } else {
        printf("%s Passed: Initial status is OK.\n", testName);
        return 0;
    }
}

int testAppsUpdate(float pos1, float pos2, AppsStatus desiredStatus, const char* testName) {
    Apps apps;
    initApps(&apps, 0, 0, 1);

    setAppPos(apps.app[0], pos1);
    setAppPos(apps.app[1], pos2);
    updateApps(&apps);

    if (apps.status != desiredStatus) {
        printf("%s Failed: Status is not desired.\n", testName);
        return 1;
    } else {
        printf("%s Passed: Status is desired.\n", testName);
        return 0;
    }
}

int main() {
    int result = 0;

    result += testAppsInit("Initialization Test");

    // Normal Operation
    result += testAppsUpdate(0, 0, APPS_OK, "Normal Operation at 0%.");
    result += testAppsUpdate(50, 50, APPS_OK, "Normal Operation at 50%.");
    result += testAppsUpdate(100, 100, APPS_OK, "Normal Operation at 100%.");

    // Edge of Acceptable Difference (10%)
    result += testAppsUpdate(0, 10, APPS_OK, "Edge Difference at 0% - 10%.");
    result += testAppsUpdate(50, 60, APPS_OK, "Edge Difference at 50% - 60%.");
    result += testAppsUpdate(90, 100, APPS_OK, "Edge Difference at 90% - 100%.");

    // Implausibility Due to Difference >10%
    result += testAppsUpdate(0, 10.01, APPS_FAULT, "Min Implausibility at 10.01%.");
    result += testAppsUpdate(50, 61, APPS_FAULT, "Implausibility at 11%.");
    result += testAppsUpdate(0, 100, APPS_FAULT, "Implausibility at 100%.");

    // Sensor Failure (Out of Range)
    result += testAppsUpdate(-1, 50, APPS_FAULT, "Sensor Failure: pos1 out of range (-1%).");
    result += testAppsUpdate(50, -1, APPS_FAULT, "Sensor Failure: pos2 out of range (-1%).");
    result += testAppsUpdate(101, 50, APPS_FAULT, "Sensor Failure: pos1 out of range (101%).");
    result += testAppsUpdate(50, 101, APPS_FAULT, "Sensor Failure: pos2 out of range (101%).");

    // Both Sensors Failed
    result += testAppsUpdate(-1, -1, APPS_FAULT, "Both Sensors Failed (-1%).");
    result += testAppsUpdate(101, 101, APPS_FAULT, "Both Sensors Failed (101%).");

    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}