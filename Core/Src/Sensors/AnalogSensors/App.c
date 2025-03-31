#include "../../../Inc/Sensors/AnalogSensors/App.h"

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
    float rawData = getAnalogSensorData(&myApp->base);

    #ifdef DEBUGn
    printf("App::update rawVal: %f\r\n", rawData);
    #endif

    myApp->position = transferFunctionApp(rawData);
}

float transferFunctionApp(float rawVal) {
    // This is a +-45 degree sensor
    if (rawVal < 0.5) {
        printf("App::transfer_function rawValue is too low\r\n");
        return -1;
    }
    else if (rawVal > 4.5) {
        printf("App::transfer_function rawValue is too high\r\n");
        return -1;
    }
    return (rawVal - 0.5)/4.0;
}
