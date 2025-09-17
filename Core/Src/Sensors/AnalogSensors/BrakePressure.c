#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"
#include "../../../Inc/Utils/MessageFormat.h"
#include "../../../Inc/Utils/Telemetry.h"

#include <stdio.h>

// Constants for brake pressure calculations
static const float kOffsetVoltage = 0.5; // Volts
static const float kVoltsPerPSIA = 0.002;
static const float kLowOutputSaturation = 0.45;  // Output saturation for undersupplied sensor
static const float kHighOutputSaturation = 4.65; // Output saturation for oversupplied sensor

void initBrakePressure(BrakePressure* bp, int hz, int channel, char* name) {
    initAnalogSensor(&bp->base, name, hz, channel, bp);
    bp->pressure = -1;
    bp->base.sensor.updateable.update = updateBrakePressure;

    // Setup telemetry signals
    bp->telem_raw = registerTelemetrySignal(name, TELEMETRY_SENSOR, UNIT_VOLTS, 1000/hz, 0.4f, 4.7f);
    bp->telem_psi = registerTelemetrySignal(name, TELEMETRY_SENSOR, UNIT_PSI, 1000/hz, 0.0f, 2000.0f);
}

float getBrakePressure(BrakePressure* bp) {
    return bp->pressure;
}

void updateBrakePressure(void* bp) {
    BrakePressure *brakePressure = (BrakePressure *)bp;
    float rawData = getAnalogSensorData(&brakePressure->base);

    // Send Telemetry float
    sendTelemetryValue(brakePressure->telem_raw, rawData);

    brakePressure->pressure = transferFunctionBrakePressure(rawData);
    if (brakePressure->pressure == -1) {
        sendMessage(brakePressure->base.sensor.updateable.name, MSG_WARNING, 
                   "Brake pressure reading below sensor minimum (%.3f V)", rawData);
    } else if (brakePressure->pressure == 2001) {
        sendMessage(brakePressure->base.sensor.updateable.name, MSG_WARNING, 
                   "Brake pressure reading above sensor maximum (%.3f V)", rawData);
    }
    
    sendTelemetryValue(brakePressure->telem_psi, brakePressure->pressure);
}

float transferFunctionBrakePressure(float rawVal) {
    if (rawVal < kLowOutputSaturation) {
        return -1;
    }
    else if (rawVal > kHighOutputSaturation) {
        return 2001;
    }
    else {
        return ((rawVal - kOffsetVoltage) / kVoltsPerPSIA);
    }
}
