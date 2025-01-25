#include "../Inc/Systems/Controller/BrakeSystemControl.h"

int testBrakeSystemControlInit(int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, const char* testName){
    BrakeSystemControl bsc;
    initBrakeSystemControl(&bsc, 0, maxTemp, brakeLightActivationPoint, heavyBrakingActivationPoint, 0, 1, 2);

    if (bsc.status != BRAKES_OK){
        printf("%s Failed: Initial status is not OK.\n", testName);
        return 1;
    }
    else{
        printf("%s Failed: Initial status is OK.\n", testName);
        return 0;
    }
}

int testFrontPressure(int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, float pressure, BrakeSystemStatus desiredStatus, const char* testName){
    BrakeSystemControl bsc;
    initBrakeSystemControl(&bsc, 0, maxTemp, brakeLightActivationPoint, heavyBrakingActivationPoint, 0, 1, 2);
    setFrontPressure(&bsc, pressure);

    BrakeSystemStatus status = checkSensorLimits(&bsc);
    if (status != desiredStatus){
        printf("%s Failed: Status after update is incorrect, actual status: %d.\n", testName, status);
        return 1;
    }
    else{
        printf("%s Passed: Status after update is correct.\n", testName);
        return 0;
    }
}

int testRearPressure(int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, float pressure, BrakeSystemStatus desiredStatus, const char* testName){
    BrakeSystemControl bsc;
    initBrakeSystemControl(&bsc, 0, maxTemp, brakeLightActivationPoint, heavyBrakingActivationPoint, 0, 1, 2);
    setRearPressure(&bsc, pressure);

    BrakeSystemStatus status = checkSensorLimits(&bsc);
    if (status != desiredStatus){
        printf("%s Failed: Status after update is incorrect, actual status: %d.\n", testName, status);
        return 1;
    }
    else{
        printf("%s Passed: Status after update is correct.\n", testName);
        return 0;
    }
}

int testTemperature(int maxTemp, int brakeLightActivationPoint, int heavyBrakingActivationPoint, float temperature, BrakeSystemStatus desiredStatus, const char* testName){
    BrakeSystemControl bsc;
    initBrakeSystemControl(&bsc, 0, maxTemp, brakeLightActivationPoint, heavyBrakingActivationPoint, 0, 1, 2);
    setTemperature(&bsc, temperature);

    BrakeSystemStatus status = checkSensorLimits(&bsc);
    if (status != desiredStatus){
        printf("%s Failed: Status after update is incorrect, actual status: %d.\n", testName, status);
        return 1;
    }
    else{
        printf("%s Passed: Status after update is correct.\n", testName);
        return 0;
    }
}


int main(){
    int tests_failed = 0;

    tests_failed += testBrakeSystemControlInit(0, 0, 0, "Initialization Test");
    tests_failed += testFrontPressure(0, 0, 0, -1, PRESSURE_UNDER_LIMIT, "Front Pressure Below Limit Test");
    tests_failed += testFrontPressure(0, 0, 0, 2001, PRESSURE_OVER_LIMIT, "Front Pressure Above Limit Test");
    tests_failed += testFrontPressure(0, 0, 0, 1234, BRAKES_OK, "Front Pressure Within Limits Test");
    tests_failed += testRearPressure(0, 0, 0, -1, PRESSURE_UNDER_LIMIT, "Rear Pressure Below Limit Test");
    tests_failed += testRearPressure(0, 0, 0, 2001, PRESSURE_OVER_LIMIT, "Rear Pressure Above Limit Test");
    tests_failed += testRearPressure(0, 0, 0, 1234, BRAKES_OK, "Rear Pressure Within Limits Test");
    tests_failed += testTemperature(1000, 0, 0, 1001, TEMPERATURE_OVER_LIMIT, "Temperature Above Limit Test");
    tests_failed += testTemperature(1000, 0, 0, 123, BRAKES_OK, "Temperature Within Limit Test");
    tests_failed += testTemperature(1000, 0, 0, -1, TEMPERATURE_SENSOR_ERROR, "Temperature Negative Test");

    if (tests_failed == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed.\n");
    }

    return tests_failed;
}