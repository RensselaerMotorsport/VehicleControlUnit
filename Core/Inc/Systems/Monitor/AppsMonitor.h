#ifndef RENSSELAERMOTORSPORT_APPS_MONITOR_H
#define RENSSELAERMOTORSPORT_APPS_MONITOR_H

#include "../MonitorSystem.h"
#include "../Controller/Apps.h"

typedef struct {
    MonitorSystem base;
    Apps* apps;
    AppsStatus status;
} AppsMonitor;

void initAppsMonitor(AppsMonitor* am, Apps* apps, int hz);
int startAppsMonitor(AppsMonitor* am);
int checkAppsMonitor(void* am);

/**
 * @brief Checks App limits for faults
 *
 * Checks if apps are more than 10% out of sync and if the values are reasonable.
 * Position is in % of pedal travel.
 *
 * @param apps A pointer to the APPs structure.
 */
void checkAppsLimit(Apps* apps);

#endif // RENSSELAERMOTORSPORT_APPS_MONITOR_H