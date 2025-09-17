#include "../../../Inc/Sensors/AnalogSensors/App.h"
#include "../../../Inc/Utils/MessageFormat.h"

#include <stdio.h>

void initApp(App* app, int hz, int channel, char* name) {
    initAnalogSensor(&app->base, name, hz, channel, app);
    app->position = 0;
    app->base.sensor.updateable.update = updateApp;

    // Register telemetry signal
    app->telem_raw = registerTelemetrySignal(name, TELEMETRY_SENSOR, UNIT_VOLTS, 1000/hz, 0.4f, 4.7f);
    app->telem_position = registerTelemetrySignal(name, TELEMETRY_SENSOR, UNIT_PERCENT, 1000/hz, 0.0f, 100.0f);

}

float getAppPosition(App* app) {
    return app->position;
}

void updateApp(App* app) {
    // FIXME: Implement APP connection with stm
    float rawData = getAnalogSensorData(&app->base);
    // Send Telemetry float
    sendTelemetryValue(app->telem_raw, rawData);
    app->position = transferFunctionApp(rawData);

    // Send diagnostic info to the debugger
    if (app->position == -1) {
        sendMessage(app->base.sensor.updateable.name, MSG_WARNING, 
                   "APP reading below sensor minimum (%.3f V)", rawData);
    } else if (app->position == -2) {
        sendMessage(app->base.sensor.updateable.name, MSG_WARNING, 
                   "APP reading above sensor maximum (%.3f V)", rawData);
    }

    sendTelemetryValue(app->telem_position, app->position * 100); // Convert from 1-scale to percentage

}

float transferFunctionApp(float rawVal) {
    // This is a +-45 degree sensor
    if (rawVal < 0.5) {
        return -1;
    }
    else if (rawVal > 4.5) {
        return -2;
    }
    return (rawVal - 0.5)/4.0;
}
