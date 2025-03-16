#include "../Inc/Systems/Controller/Apps.h"

#include <stdio.h>
#include <assert.h>

int testAppsInit(const char* testName) {
    Apps apps;
    initApps(&apps, 0, 0, 1);

    if (apps.status != APPS_OK) {
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 0;
    } else {
        printf("%s Passed: Initial status is OK.\n", testName);
        return 1;
    }
}

int testAppsUpdate(float pos1, float pos2, AppsStatus desiredStatus,
                   const char* testName) {
    Apps apps;
    initApps(&apps, 0, 0, 1);

    setAppPos(apps.app[0], pos1);
    setAppPos(apps.app[1], pos2);
    updateApps(&apps);

    if (apps.status != desiredStatus) {
        printf("%s Failed: Status is not desired.\n", testName);
        return 0;
    } else {
        printf("%s Passed: Status is desired.\n", testName);
        return 1;
    }
}

int testGetAppsPosition(float pos1, float pos2, float desiredPos,
                        const char* testName) {
    Apps apps;
    initApps(&apps, 0, 0, 1);

    setAppPos(apps.app[0], pos1);
    setAppPos(apps.app[1], pos2);
    updateApps(&apps);
    float outPos = getAppsPosition(&apps);

    if (outPos != desiredPos) {
        printf("%s Failed: Position is not desired.\n", testName);
        return 0;
    } else {
        printf("%s Passed: Position is desired.\n", testName);
        return 1;
    }
}

int apps_main() {
    assert(testAppsInit("Initialization Test"));

    // Normal Operation
    assert(testAppsUpdate(0, 0, APPS_OK, "Normal Operation at 0%."));
    assert(testAppsUpdate(50, 50, APPS_OK, "Normal Operation at 50%."));
    assert(testAppsUpdate(100, 100, APPS_OK, "Normal Operation at 100%."));

    // Edge of Acceptable Difference (10%)
    assert(testAppsUpdate(0, 10, APPS_OK, "Edge Difference at 0% - 10%."));
    assert(testAppsUpdate(50, 60, APPS_OK, "Edge Difference at 50% - 60%."));
    assert(testAppsUpdate(90, 100, APPS_OK, "Edge Difference at 90% - 100%."));

    // Implausibility Due to Difference >10%
    assert(testAppsUpdate(0, 10.01, APPS_FAULT, "Min Implausibility at 10.01%."));
    assert(testAppsUpdate(50, 61, APPS_FAULT, "Implausibility at 11%."));
    assert(testAppsUpdate(0, 100, APPS_FAULT, "Implausibility at 100%."));

    // Sensor Failure (Out of Range)
    assert(testAppsUpdate(-1, 50, APPS_FAULT, "Sensor Failure: pos1 out of range -1%."));
    assert(testAppsUpdate(50, -1, APPS_FAULT, "Sensor Failure: pos2 out of range -1%."));
    assert(testAppsUpdate(101, 50, APPS_FAULT, "Sensor Failure: pos1 out of range 101%."));
    assert(testAppsUpdate(50, 101, APPS_FAULT, "Sensor Failure: pos2 out of range 101%."));

    // Both Sensors Failed
    assert(testAppsUpdate(-1, -1, APPS_FAULT, "Both Sensors Failed -1%."));
    assert(testAppsUpdate(101, 101, APPS_FAULT, "Both Sensors Failed 101%."));

    // Normal Position
    assert(testGetAppsPosition(0, 0, 0, "Average Position at 0%"));
    assert(testGetAppsPosition(50, 60, 55, "Average Position at 55%"));
    assert(testGetAppsPosition(55, 56, 55.5, "Average Position at 55.5%"));

    // Position with fault
    assert(testGetAppsPosition(0, 100, 0.0f, "Position Implausibility at 100%."));
    assert(testGetAppsPosition(50, 60.01, 0.0f, "Position Implausibility at 100%."));

    // Position from sensor failure
    assert(testGetAppsPosition(-1, -1, 0.0f, "Position Both Sensors Failed: out of range -1%."));
    assert(testGetAppsPosition(101, 101, 0.0f, "Position Both Sensors Failed: out of range 101%."));
    assert(testGetAppsPosition(-1, 10, 0.0f, "Position Sensor Failure: out of range -1%."));


    printf("All tests passed.\n");
    return 0;
}
