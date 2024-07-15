#include "../includes/TorqueControlActuator.h"

int testTorqueControlInitialization(float maxTorque, const char* testName) {
    TorqueControlActuator tc;
    initTorqueControlActuator(&tc, 0, 0, maxTorque);

    if (tc.status != TORQUE_OK) {
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 1;
    } else {
        printf("%s Passed: Initial status is OK.\n", testName);
        return 0;
    }
}

int testTorqueUpdate(float torque, float desiredTorque,
                     TorqueStatus desiredStatus, const char* testName) {
    TorqueControlActuator tc;
    float maxTorque = 100.0f;
    initTorqueControlActuator(&tc, 0, 0, maxTorque);

    setTorque(&tc, torque);
    setDesiredTorque(&tc, desiredTorque);
    updateTorqueControlActuator(&tc);
    TorqueStatus status = checkTorqueLimits(&tc);

    if (status != desiredStatus) {
        printf("%s Failed: Status after update is incorrect, actual status: %d.\n",
               testName, status);
        return 1;
    } else {
        printf("%s Passed: Status after update is correct.\n", testName);
        return 0;
    }
}

// Main function to run all tests
int main() {
    int result = 0;

    result += testTorqueControlInitialization(100.0f, "Initialization Test");
    result += testTorqueUpdate(75.0f, 75.0f, TORQUE_OK, "Update Test Normal");
    result += testTorqueUpdate(93.5f, 95.0f, TORQUE_OK, "Update Test Close to Limit Below");
    result += testTorqueUpdate(98.5f, 95.0f, TORQUE_OK, "Update Test Close to Limit Above");
    result += testTorqueUpdate(101.0f, 95.0f, TORQUE_OVER_LIMIT, "Update Test Over Limit");
    result += testTorqueUpdate(-10.0f, 20.0f, TORQUE_UNDER_LIMIT, "Update Test Negative Torque");
    result += testTorqueUpdate(10.0f, 80.0f, TORQUE_SENSOR_ERROR, "Update Test Sensor Error");

    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
