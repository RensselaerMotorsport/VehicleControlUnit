#include "../../../Inc/Systems/Controller/Apps.h"
#include "../../../Inc/Utils/Common.h"

#include <math.h>

/**
 * @brief Checks App limits for faults
 *
 * Checks if apps are more than 10% out of sync and if the values are reasonable.
 * Position is in % of pedal travel.
 *
 * @param apps A pointer to the APPs structure.
 */
void checkAppsLimit(Apps* apps) {
    float pos1 = getAppPosition(apps->app[0]);
    float pos2 = getAppPosition(apps->app[1]);

    // Resonable Value check
    if (pos1 > 100 || pos1 < 0 || pos2 > 100 || pos2 < 0) {
        apps->status = APPS_FAULT;
    }

    // In sync check
    float difference = fabs(pos1 - pos2);
    if (difference > APPS_DIFFERENCE * 100) {
        apps->status = APPS_FAULT;
    }
}

void initApps(Apps* apps, int hz, int channel1, int channel2) {
    // Allocate memory for the app instances
    apps->app[0] = (App*)malloc(sizeof(App));
    apps->app[1] = (App*)malloc(sizeof(App));

    // Check if memory allocation was successful
    if (apps->app[0] == NULL || apps->app[1] == NULL) {
        return;
    }

    // System setup
    MonitorSystem monitor;
    initMonitorSystem(&monitor, "APPs Monitor", 200, m_APPS, VEHICLE_SHUTDOWN);
    initControllerSystem(&apps->base, "APPS", hz, c_APPS);
    defaultAddMonitor(&apps->base, &monitor);

    // Controller setup
    apps->base.system.updateable.update = updateApps;
    apps->base.safety = appsSafetyCheck;
    apps->status = APPS_OK;
    initApp(apps->app[0], hz, channel1);
    initApp(apps->app[1], hz, channel2);
}

void updateApps(void* apps) {
    Apps* appsPtr = (Apps*) apps;
    updateApp(&appsPtr->app[0]);
    updateApp(&appsPtr->app[1]);
    if (appsPtr->base.safety(apps) == FAILURE) {
        printf("APPs is not in a safe state\n");
        return;
    }
    checkAppsLimit(appsPtr);
    if (appsPtr->status != APPS_OK) return;
}

Updateable* GetUpdateableApps(Apps* apps) {
    return &(apps->base.system.updateable);
}

float getAppsPosition(Apps* apps) {
    checkAppsLimit(apps);
    if (apps->status != APPS_OK) return 0.0f;
    float pos1 = getAppPosition(apps->app[0]);
    float pos2 = getAppPosition(apps->app[1]);
    return fabs(pos1 + pos2) / 2;
}

int appsSafetyCheck(void* apps) {
    Apps* appsPtr = (Apps*)apps;
    if(appsPtr->base.num_monitors == 0) {
        printf("No monitors set for Apps\n");
        return FAILURE;
    }
    else if (appsPtr->status != APPS_OK) {
        printf("Apps is not in a safe state\n");
        return FAILURE;
    }
    return SUCCESS;
}
