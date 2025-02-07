#include "../../../Inc/Systems/Monitor/AppsMonitor.h"
#include "../../../Inc/Utils/Common.h"

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
        return FAILURE;
    }
    ENABLE(am->base.system);
    return SUCCESS;
}

// Check the Apps MonitorSystem
int checkAppsMonitor(void* am) {
    AppsMonitor* amPtr = (AppsMonitor*)am;
    Apps* apps = amPtr->apps;
    if (apps->status != APPS_OK) {
        return FAILURE;
    }
    checkAppsLimit(apps);
    if (apps->status != APPS_OK) {
        return FAILURE;
    }
    return SUCCESS;
}


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
