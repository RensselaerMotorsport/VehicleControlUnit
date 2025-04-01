#include "../../../Inc/Utils/Common.h"
#include "../../../Inc/Systems/External/Inverter.h"

#include <stdio.h>  // For printf

void initInverter(Inverter* inverter, TorqueControl* tc, int hz, int maxCurrent, int maxTemp, int maxVoltage) {
    initExternalSystem(&inverter->base, "Inverter", hz, e_INVERTER, updateInverter, checkInverterHeartbeat, inverter);
    inverter->tc = tc;
    inverter->maxCurrent = maxCurrent;
    inverter->maxTemp = maxTemp;
    inverter->maxVoltage = maxVoltage;
}

int updateInverter(ExternalSystem* external) {
    Inverter* inverter = (Inverter*)external->child;
    // Check if torque control is validated
    if (inverter->tc->base.state != c_validated) {
        printf("Inverter: Torque Control Actuator is not validated\r\n");
        return _FAILURE;
    }

    
    #ifdef DEBUGn
    printf("Inverter updated. Torque: %f, Current: %d, Temp: %d, Voltage: %d\r\n",
           inverter->tc->desiredTorque, inverter->maxCurrent, inverter->maxTemp, inverter->maxVoltage);
    #endif
    
    dac1_buffer[0] = ((inverter->tc->desiredTorque) / (inverter->tc->maxAllowedTorque)) * 4096.0;

    return _SUCCESS;
}

int checkInverterHeartbeat(void* self) {
    Inverter* inverter = (Inverter*)self;
    // Check if the inverter is still alive
    return _SUCCESS;
}
