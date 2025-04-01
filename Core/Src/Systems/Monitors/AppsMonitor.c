#include "../../../Inc/Systems/Monitor/AppsMonitor.h"
#include "../../../Inc/Utils/Common.h"
#include <math.h>

// Make new Apps MonitorSystem
void initAppsMonitor(AppsMonitor* am, Apps* apps, int hz) {
    initMonitorSystem(&am->base, "Apps Monitor", hz, m_APPS, VEHICLE_SHUTDOWN, checkAppsMonitor);
    am->apps = apps;
    am->status = APPS_OK;
}

// Start the Apps MonitorSystem
int startAppsMonitor(AppsMonitor* am) {
    if (am->base.runMonitor == NULL) {
        printf("Monitor function not set for AppsMonitor\n");
        return _FAILURE;
    }
    ENABLE(am->base.system);
    return _SUCCESS;
}

// Check the Apps MonitorSystem
int checkAppsMonitor(void* am) {
    AppsMonitor* amPtr = (AppsMonitor*)am;
    Apps* apps = amPtr->apps;
    checkAppsLimit(apps);
    if (apps->status != APPS_OK && apps->status != APPS_LOW) {
        return _FAILURE;
    }
    if (apps->status == APPS_LOW) {
        printf("AppsMonitor: APPS position is too low. Defaulting to 0 to pass\r\n");
    }
    return _SUCCESS;
}


void checkAppsLimit(Apps* apps) {
    float pos1 = getAppPosition(apps->app[0]);
    float pos2 = getAppPosition(apps->app[1]);

    // Resonable Value check upper bound
    if (pos1 > 100 || pos2 > 100) {
        apps->status = APPS_FAULT;
        return;
    }

    // In sync check
    float difference = fabs(pos1 - pos2);
    if (difference > APPS_DIFFERENCE * 100) {
        apps->status = APPS_FAULT;
        return;
    } 

    // Resonable Value check lower bound
    if (pos1 < 0 || pos2 < 0) {
        apps->status = APPS_LOW;
        return;
    }

    apps->status = APPS_OK;
}
