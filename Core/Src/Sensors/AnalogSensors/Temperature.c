#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initTemperature(Temperature* temperature, int hz, int channel) {
    initAnalogSensor(&temperature->base, "Temperature", hz, channel, temperature);
    temperature->degrees = 0;
    temperature->base.sensor.updateable.update = updateTemperature;
}

void updateTemperature(void* temperature) {
    Temperature *myTemp = (Temperature *)temperature;
    float rawData = getAnalogSensorData(&myTemp->base);
    myTemp->degrees = transferFunction(temperature, rawData);
}

double getTemperatureCelsius(Temperature* temperature) {
    return temperature->degrees;
}

double getTemperatureFahrenheit(Temperature* temperature) {
    return temperature->degrees * 9.0 / 5.0 + 32;
}

double transferFunction(Temperature* temperature, float rawVal) {
    
    #ifdef DEBUGn
    printf("Implement Temperature::TF\r\n");
    #endif

    return 0.0;
}
