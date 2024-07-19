#ifndef RENSSELAERMOTORSPORT_TORQUE_CONTROL_H
#define RENSSELAERMOTORSPORT_TORQUE_CONTROL_H

#include "ControllerSystem.h"

#define TORQUE_ERROR_MARGIN 0.05 // TODO: Determine correct value for the margin, or change to lookup table if needed.

// TODO: Move to torque police in the future
typedef enum {
    TORQUE_OK,
    TORQUE_OVER_LIMIT,
    TORQUE_UNDER_LIMIT,
    TORQUE_SENSOR_ERROR
} TorqueStatus;

typedef struct {
    ControllerSystem base;
    float actualTorque;
    float desiredTorque;
    float maxAllowedTorque;
    TorqueStatus status;
} TorqueControl;

void initTorqueControl(TorqueControl* tc, int hz, float maxTorque);
void setDesiredTorque(TorqueControl* tc, float torque);
void setTorque(TorqueControl* tc, float torque);
void updateTorqueControl(void* tc);
TorqueStatus checkTorqueLimits(TorqueControl* tc);
void sendTorqueCommand(const TorqueControl* tc);

#endif
