#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initTemperature(Temperature* temp, int hz, int channel) {
    initAnalogSensor(&temp->base, "Temperature", hz, channel);
    temp->base.sensor.updateable.update = updateTemperature;
    temp->degrees = 0;
}

void updateTemperature(void* temp) {
    printf("Implement Temperature::update\n");
    Temperature *myTemp = (Temperature *)temp;
    myTemp->degrees = transferFunction(temp, 0);
}

double getTemperatureCelsius(Temperature* temp) {
    return temp->degrees;
}

double getTemperatureFahrenheit(Temperature* temp) {
    return temp->degrees * 9.0 / 5.0 + 32;
}

double transferFunction(Temperature* temp, int rawVal) {
    return 0.0;
}
