#include "../../../Inc/Systems/Controller/Apps.h"

void initApps(Apps* apps, int hz, int channel1, int channel2) {
    initControllerSystem(&app->base, "APPS", hz, c_APPS);
    apps->base.system.updateable.update = updateAPPS;
    initApp(apps->app[0], hz, channel1);
    initApp(apps->app[1], hz, channel2);
}

void updateApps(void* apps) {
    TODO: implement app transfer func.
    Apps* myApps = (Apps*) apps;
    updateApp(&myApps.app[0]);
    updateApp(&myApps.app[1]);
}
