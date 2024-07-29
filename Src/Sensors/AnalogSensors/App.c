#include "../../../includes/App.h"
#include <stdio.h>

void initApp(App* app, int hz, int channel) {
    initAnalogSensor(&app->base, "App", hz, channel);
    app->position = 0;
    app->base.sensor.updateable.update = updateApp;
}

float getAppPosition(App* app) {
    return app->position;
}

void updateApp(void* app) {
    App *myApp = (App *)app;
    printf("Implement App::update\n");
    float rawData = 0.0f;
    myApp->position = transferFunctionApp(rawData);
}

float transferFunctionApp(float rawVal) {
    // This is a +-45 degree sensor
    if (rawVal < 0.5) {
        printf("App::transfer_function rawValue is too low\n");
        return -1;
    } else if (rawVal > 4.5) {
        printf("App::transfer_function rawValue is too high\n");
        return -1;
    }
    return (22.5 * rawVal) - 56.25;
}
