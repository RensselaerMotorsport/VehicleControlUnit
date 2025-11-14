#include "../../../Inc/Systems/Monitor/AppsMonitor.h"
#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Utils/MessageFormat.h"
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
        //printf("Monitor function not set for AppsMonitor\n");
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
    return _SUCCESS;
}


void checkAppsLimit(Apps* apps) {
    float pos1 = getAppPosition(apps->app[0]);
    float pos2 = getAppPosition(apps->app[1]);

    // Resonable Value check upper bound
    if (pos1 > 1 || pos2 > 1) {
        apps->status = APPS_FAULT;
        return;
    }

    // In sync check
    float difference = fabs(pos1 - pos2);
    if (difference > APPS_DIFFERENCE) {
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
