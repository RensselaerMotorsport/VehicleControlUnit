#include "../../../Inc/Systems/Controller/TorqueControl.h"
#include "../../../Inc/Systems/ControllerSystem.h"
#include "../../../Inc/Utils/Common.h"
#include <math.h>

void initTorqueControl(TorqueControl* tc, Apps* apps, int hz, float maxTorque) {
    initControllerSystem(&tc->base, "Torque Control", hz, c_TORQUE, setDesiredTorque, tc);
    tc->desiredTorque = 0;
    tc->maxAllowedTorque = maxTorque;
    tc->apps = apps;
}

int startTorqueControl(TorqueControl* tc) {
    if (tc->base.safety == NULL) {
        printf("Safety system not set for Torque Control\n");
        return _FAILURE;
    }
    else if (tc->base.safety(&tc->base) == _FAILURE) {
        printf("Torque Control Actuator is not in a safe state\n");
        return _FAILURE;
    }
    ENABLE(tc->base.system);
    tc->base.state = c_idle;
    return _SUCCESS;
}

int setDesiredTorque(ControllerSystem* controller) {
    TorqueControl* tc = (TorqueControl*)controller->child;
    float pedalposition = getAppsPosition(tc->apps);
    float torque = pedalposition * tc->maxAllowedTorque;

    if (torque > tc->maxAllowedTorque) {
        printf("Desired torque exceeds the maximum allowed torque, stepping down to max\n");
        torque = tc->maxAllowedTorque;
    }

    // Perform any desired mapping, i.e. fit to sigmoid function.
    double normalized = torque / tc->maxAllowedTorque;  // Normalize to range [0, 1]
    double s_curve = 1.0 / (1.0 + exp(-10 * (normalized - 0.5)));
    tc->desiredTorque = s_curve * tc->maxAllowedTorque;
    tc->base.state = c_computed;
    
    #ifdef DEBUGn
    printf("Desired Torque: %f\r\n", tc->desiredTorque);
    #endif

    return _SUCCESS;
}
