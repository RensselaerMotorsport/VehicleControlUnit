#include "../../../includes/MotorController.h"

void initMotorController(MotorController* mc, Esc* esc, App* app) {
    if (mc != NULL) {
        mc->esc = esc;
        mc->app = app;
    }
}

float getPedalPosition(const MotorController* mc) {
    if (mc && mc->app) {
        return getAppPosition(mc->app);  // Assuming getAppPosition is implemented in App.c
    }
    return 0.0;  // Return default value if no app
}

float getMotorTemp(const MotorController* mc) {
    if (mc && mc->esc) {
        return getEscTemp(mc->esc);  // Assuming getEscTemp is implemented in Esc.c
    }
    return 0.0;  // Return default value if no esc
}
