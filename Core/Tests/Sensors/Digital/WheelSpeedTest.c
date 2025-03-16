#include <stdio.h>
#include <math.h>  // for fabs function
#include "../Inc/Sensors/DigitalSensors/WheelSpeed.h"

int testTransferFunction(float radius, int numTeeth, int pulseCount,
                         float timeInterval, float expectedSpeed,
                         const char* testName) {
    WheelSpeed wheelSpeed;
    int wheelSpeedHz = 200;
    initWheelSpeed(&wheelSpeed, wheelSpeedHz, 0, radius, numTeeth, 0);

    addPulse(&wheelSpeed, pulseCount);
    // Inverting time because get function relies on current time
    setTimeInterval(&wheelSpeed, -timeInterval);

    float speed = calculateSpeed(&wheelSpeed);
    // NOTE: Speed may vary based on current time being output.
    if (fabs(speed - expectedSpeed) <= 0.05) {
        printf("%s Passed: Speed: %.2f mph (Expected: %.2f mph)\n",
               testName, speed, expectedSpeed);
        return 0;
    } else {
        printf("%s Failed: Speed: %.2f mph (Expected: %.2f mph)\n",
               testName, speed, expectedSpeed);
        return 1;
    }
}

int wheel_speed_main() {
    int result = 0;

    // Low speed tests
    result += testTransferFunction(0.127f, 48, 100, 0.5f, 7.44f, "Low Speed Test 1");
    result += testTransferFunction(0.15f, 60, 100, 1.0f, 3.51f, "Low Speed Test 2");
    result += testTransferFunction(0.1f, 40, 25, 0.25f, 3.51f, "Low Speed Test 3");

    // Medium speed tests
    result += testTransferFunction(0.127f, 48, 300, 0.5f, 22.31f, "Medium Speed Test 1");
    result += testTransferFunction(0.15f, 60, 300, 1.0f, 10.54f, "Medium Speed Test 2");
    result += testTransferFunction(0.1f, 40, 150, 0.25f, 21.08f, "Medium Speed Test 3");

    // High speed tests
    result += testTransferFunction(0.127f, 48, 1000, 0.5f, 74.37f, "High Speed Test 1");
    result += testTransferFunction(0.15f, 60, 1000, 1.0f, 35.14f, "High Speed Test 2");
    result += testTransferFunction(0.1f, 40, 500, 0.25f, 70.28f, "High Speed Test 3");

    if (result == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return result;
}
