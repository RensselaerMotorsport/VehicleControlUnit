#include "../../../Inc/Systems/Controller/Apps.h"
#include "../../../Inc/Utils/Common.h"

#include <math.h>

void initApps(Apps* apps, int hz, int channel1, int channel2) {
    initControllerSystem(&apps->base, "Apps", hz, c_APPS, updateApps, apps);
    // Allocate memory for the app instances
    apps->app[0] = (App*)malloc(sizeof(App));
    apps->app[1] = (App*)malloc(sizeof(App));

    // Check if memory allocation was successful
    if (apps->app[0] == NULL || apps->app[1] == NULL) {
        return;
    }

    // Controller setup
    apps->status = APPS_OK;
    initApp(apps->app[0], hz, channel1);
    initApp(apps->app[1], hz, channel2);
    ENABLE(apps->base.system);
}

int updateApps(void* self) {
    Apps* appsPtr = (Apps*) self;
    updateApp(appsPtr->app[0]);
    updateApp(appsPtr->app[1]);

    // Set to computed
    appsPtr->base.state = c_computed;
    
    
    #ifdef DEBUGn
    printf("Apps updated. #1: %f, #2: %f\r\n", getAppPosition(appsPtr->app[0]), getAppPosition(appsPtr->app[1]));
    #endif

    return _SUCCESS;
}

float getAppsPosition(Apps* apps) {
    if (apps->base.state != c_validated) {
        
        printf("Apps Controller value has not been validated\r\n");
        return 0.0f;
    }
    checkAppsLimit(apps);
    if (apps->status != APPS_OK) return 0.0f;
    float pos1 = getAppPosition(apps->app[0]);
    float pos2 = getAppPosition(apps->app[1]);
    return fabs(pos1 + pos2) / 2;
}
