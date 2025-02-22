#include "../../../Inc/Outputs/DigitalOutput.h"
#include "../../../Inc/Systems/Controller/BrakeSystemControl.h"
#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Sensors/AnalogSensors/BrakePressure.h"
#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initBrakeSystemControl(BrakeSystemControl *bsc, int hz, int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, int fbp_channel, int rbp_channel, int temp_channel, int light_port){
    initControllerSystem(&bsc-> base, "Brake System Control", hz, c_BRAKES, updateBrakeSystemControl, bsc);
    bsc->frontPressure = (BrakePressure *)malloc(sizeof(BrakePressure));
    bsc->rearPressure = (BrakePressure *)malloc(sizeof(BrakePressure));
    bsc->temperature = (Temperature *)malloc(sizeof(Temperature));
    bsc->brakeLight = (DigitalOutput *)malloc(sizeof(DigitalOutput));
    bsc -> maxTemperatureAllowed = maxTemp;
    initBrakePressure(bsc -> frontPressure, hz, fbp_channel);
    initBrakePressure(bsc -> rearPressure, hz, rbp_channel);
    bsc -> minPressure = 0;
    bsc -> maxPressure = 2000;
    initTemperature(bsc -> temperature, hz, temp_channel);
    bsc -> brakeLightActivationPoint = brakeLightActivationPoint;
    bsc -> brakeLightActive = 0;
    bsc -> brakeLightBlink = 0;
    initDigitalOutput(bsc -> brakeLight, "Brake Light", hz, light_port);
    bsc -> heavyBrakingActivationPoint = heavyBrakingActivationPoint;
    bsc -> heavyBraking = 0;
    bsc -> status = BRAKES_OK;
}

int startBrakeSystemControl(BrakeSystemControl *bsc) {
    if (bsc -> base.safety == NULL){
        printf("Safety system not set for Brake System Control\r\n");
        return _FAILURE;
    }
    else if (bsc -> base.safety(bsc) == _FAILURE){
        printf("Brake System Control Actuator is not in a safe state\r\n");
        return _FAILURE;
    }
    ENABLE(bsc -> base.system);
    bsc -> base.state = c_idle;
    return _SUCCESS;
}

int updateBrakeSystemControl(void* self) {
    BrakeSystemControl *bsc = (BrakeSystemControl *)self;
    setSensorReadings(bsc);
    activateBrakeLight(bsc);
    inHeavyBreaking(bsc);
    bsc->base.state = c_computed;
    writeDigitalOutputData(bsc->brakeLight, bsc->brakeLightActive);
    
    #ifdef DEBUGn
    printf("Brake System Control updated. Front Pressure: %f, Rear Pressure: %f, Temperature: %f\r\n", getBrakePressure(bsc -> frontPressure), getBrakePressure(bsc -> rearPressure), getTemperatureFahrenheit(bsc -> temperature));
    printf("Brake Light Active: %d, Heavy Braking: %d\r\n", bsc -> brakeLightActive, bsc -> heavyBraking);
    #endif

    return _SUCCESS;
}

void setSensorReadings(BrakeSystemControl *bsc){
    updateBrakePressure(bsc->frontPressure);
    updateBrakePressure(bsc->rearPressure);
    updateTemperature(bsc->temperature);
}

void activateBrakeLight(BrakeSystemControl *bsc){
    if (getBrakePressure(bsc -> frontPressure) > bsc -> brakeLightActivationPoint 
        || getBrakePressure(bsc -> rearPressure) > bsc -> brakeLightActivationPoint){
        if (bsc->brakeLightBlink < 8) {bsc->brakeLightBlink++;}
        if (bsc->brakeLightBlink%2 == 0) {
            bsc -> brakeLightActive = 1;
            return;
        }
    } else {
        bsc -> brakeLightBlink = 0;
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
    else if (front < bsc -> minPressure || rear < bsc -> minPressure){
        return PRESSURE_UNDER_LIMIT;
    }
    else if (temp > bsc -> maxTemperatureAllowed){
        return TEMPERATURE_OVER_LIMIT;
    }
    else if (temp < 0){
        return TEMPERATURE_SENSOR_ERROR;
    } else {
        return BRAKES_OK;
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