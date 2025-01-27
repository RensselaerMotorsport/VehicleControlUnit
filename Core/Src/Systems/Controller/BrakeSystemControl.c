#include "../../../Inc/Systems/Controller/BrakeSystemControl.h"
#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"
#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, int fbp_channel, int rbp_channel, int temp_channel){
    initControllerSystem(&bsc-> base, "Brake System Control", hz, c_BRAKES);
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
    bsc -> base.safety = brakeSafteyCheck;
}

void setSensorReadings(BrakeSystemControl *bsc, float frontPressure, float rearPressure, float temperature){
    updateBrakePressure(bsc -> frontPressure);
    updateBrakePressure(bsc -> rearPressure);
    updateTemperature(bsc -> temperature);
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
    float temp = getBrakePressure(bsc -> temperature);
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

int brakeSafteyCheck(void* bsc){
    BrakeSystemControl *bscPtr = (BrakeSystemControl*)bsc;
    if (bscPtr->base.num_monitors == 0){
        printf("No monitors set for Brake System Control\n");
        return FAILURE;
    }
    else if (bscPtr->status != BRAKES_OK){
        printf("Brake System is not in a safe state\n");
        return FAILURE;
    }
    return SUCCESS;
}

//The following functions below are for testing functionality and should not be used elsewhere

void setFrontPressure(BrakeSystemControl *bsc, float *pressure){
    bsc -> frontPressure = pressure;
}

void setRearPressure(BrakeSystemControl *bsc, float *pressure){
    bsc -> rearPressure = pressure;
}

void setTemperature(BrakeSystemControl *bsc, float *temperature){
    bsc -> temperature = temperature;
}