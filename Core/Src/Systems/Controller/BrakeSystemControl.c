#include "../../../Inc/Systems/Controller/BrakeSystemControl.h"
#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"
#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, int fbp_channel, int rbp_channel, int temp_channel){
    initControllerSystem(&bsc-> base, "Brake System Control", hz, c_BRAKES, NULL);
    bsc -> maxTemperatureAllowed = maxTemp;
    initBrakePressure(bsc -> frontPressure, hz, fbp_channel);
    initBrakePressure(bsc -> rearPressure, hz, rbp_channel);
    bsc -> minPressure = 0;
    bsc -> maxPressure = 2000;
    initTemperature(bsc -> temperature, hz, temp_channel);
    bsc -> brakeLightActivationPoint = brakeLightActivationPoint;
    bsc -> brakeLightActive = 0;
    bsc -> heavyBrakingActivationPoint = heavyBrakingActivationPoint;
    bsc -> heavyBraking = 0;
    bsc -> status = BRAKES_OK;
}

void setSensorReadings(BrakeSystemControl *bsc){
    updateBrakePressure(bsc->frontPressure);
    updateBrakePressure(bsc->rearPressure);
    updateTemperature(bsc->temperature);
    inHeavyBreaking(bsc);
    activateBrakeLight(bsc);
    bsc->status = checkSensorLimits(bsc);
    bsc->base.state = c_computed;
}

void activateBrakeLight(BrakeSystemControl *bsc){
    if (getBrakePressure(bsc -> frontPressure) > bsc -> brakeLightActivationPoint || getBrakePressure(bsc -> rearPressure) > bsc -> brakeLightActivationPoint){
        bsc -> brakeLightActive = 1;
        return;
    }
    bsc -> brakeLightActive = 0;
    return;
}

void inHeavyBreaking(BrakeSystemControl *bsc){
    if (getBrakePressure(bsc -> frontPressure) > bsc -> heavyBrakingActivationPoint || getBrakePressure(bsc -> rearPressure) > bsc -> heavyBrakingActivationPoint){
        bsc -> heavyBraking = 1;
        return;
    }
    bsc -> heavyBraking = 0;
    return;
}

BrakeSystemStatus checkSensorLimits(BrakeSystemControl *bsc){
    float front = getBrakePressure(bsc -> frontPressure);
    float rear = getBrakePressure(bsc -> rearPressure);
    float temp = getTemperatureFahrenheit(bsc -> temperature);
    
    if (front > bsc -> maxPressure || rear > bsc -> maxPressure){
        return PRESSURE_OVER_LIMIT;
    }
    else if (front > bsc -> minPressure || rear > bsc -> minPressure){
        return PRESSURE_UNDER_LIMIT;
    }
    else if (temp > bsc -> maxTemperatureAllowed){
        return TEMPERATURE_OVER_LIMIT;
    }
    else if (temp < 0){
        return TEMPERATURE_SENSOR_ERROR;
    }
}

//The following functions below are for testing functionality and should not be used elsewhere

void setFrontPressure(BrakeSystemControl *bsc, float pressure){
    bsc -> frontPressure -> pressure = pressure;
    inHeavyBreaking(bsc);
    activateBrakeLight(bsc);
}

void setRearPressure(BrakeSystemControl *bsc, float pressure){
    bsc -> rearPressure -> pressure = pressure;
    inHeavyBreaking(bsc);
    activateBrakeLight(bsc);
}

void setTemperature(BrakeSystemControl *bsc, float temperature){
    bsc -> temperature -> degrees = temperature;
    inHeavyBreaking(bsc);
    activateBrakeLight(bsc);
}