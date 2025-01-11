#include "../../../Inc/Systems/Controller/BrakeSystemControl.h"
#include "../../../Inc/Utils/Common.h"

void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, float brakeLightActivationPoint, float heavyBrakingActivationPoint){
    initControllerSystem(&bsc-> base, "Brake System Control", hz, c_BRAKES);
    bsc -> maxTemperatureAllowed = maxTemp;
    bsc -> frontPressure = 0.0;
    bsc -> rearPressure = 0.0;
    bsc -> minPressure = 0.5;
    bsc -> maxPressure = 4.5;
    bsc -> temperature = 0.0;
    bsc -> brakeLightActivationPoint = brakeLightActivationPoint;
    bsc -> brakeLightActive = 0;
    bsc -> heavyBrakingActivationPoint = heavyBrakingActivationPoint;
    bsc -> heavyBraking = 0;
    bsc -> status = BRAKES_OK;
    bsc -> base.safety = brakeSafteyCheck;
}

void setSensorReadings(BrakeSystemControl *bsc, float frontPressure, float rearPressure, float temperature){
    bsc -> frontPressure = frontPressure;
    bsc -> rearPressure = rearPressure;
    bsc -> temperature = temperature;
}

void activateBrakeLight(BrakeSystemControl *bsc){
    if (bsc -> frontPressure > bsc -> brakeLightActivationPoint || bsc -> rearPressure > bsc -> brakeLightActivationPoint){
        bsc -> brakeLightActive = 1;
        return;
    }
    bsc -> brakeLightActive = 0;
    return;
}

void inHeavyBreaking(BrakeSystemControl *bsc){
    if (bsc -> frontPressure > bsc -> heavyBrakingActivationPoint || bsc -> rearPressure > bsc -> heavyBrakingActivationPoint){
        bsc -> heavyBraking = 1;
        return;
    }
    bsc -> heavyBraking = 0;
    return;
}

BrakeSystemStatus checkSensorLimits(BrakeSystemControl *bsc){
    if (bsc -> frontPressure > bsc -> maxPressure || bsc -> rearPressure > bsc -> maxPressure){
        return PRESSURE_OVER_LIMIT;
    }
    else if (bsc->frontPressure > bsc->maxPressure || bsc->rearPressure > bsc->maxPressure){
        return PRESSURE_UNDER_LIMIT;
    }
    else if (bsc->temperature > bsc->maxTemperatureAllowed){
        return TEMPERATURE_OVER_LIMIT;
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