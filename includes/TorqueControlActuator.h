#ifndef RENSSELAERMOTORSPORT_TORQUE_CONTROL_ACTUATOR_H
#define RENSSELAERMOTORSPORT_TORQUE_CONTROL_ACTUATOR_H

#include "DigitalSensor.h"

#define TORQUE_ERROR_MARGIN 0.05 // NOTE: IDK What this value should be realistically

typedef enum {
    TORQUE_OK,
    TORQUE_OVER_LIMIT,
    TORQUE_UNDER_LIMIT,
    TORQUE_SENSOR_ERROR
} TorqueStatus;

typedef struct {
    DigitalSensor base;
    float currentTorque;
    float desiredTorque;
    float maxAllowedTorque;
    TorqueStatus status;
} TorqueControlActuator;

void initTorqueControlActuator(TorqueControlActuator* tc, int hz, int port,
                               float maxTorque);
void setDesiredTorque(TorqueControlActuator* tc, float torque);
void setTorque(TorqueControlActuator* tc, float torque);
void updateTorqueControlActuator(void* tc);
TorqueStatus checkTorqueLimits(TorqueControlActuator* tc);
void sendTorqueCommand(const TorqueControlActuator* tc);

#endif
