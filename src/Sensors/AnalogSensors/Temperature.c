#include "../../../includes/Temperature.h"

void initTemperature(Temperature* temp, int hz, int channel) {
    initAnalogSensor(&temp->base, "Temperature", hz, channel);
    temp->base.base.update = updateTemperature;
    temp->degrees = 0;
}

void updateTemperature(void* temp) {
    Temperature *myTemp = (Temperature *)temp;
    myTemp->degrees = transferFunction(temp, getAnalogSensorData(&myTemp->base));
}

double getTemperatureCelsius(Temperature* temp) {
    return temp->degrees;
}

double getTemperatureFahrenheit(Temperature* temp) {
    return temp->degrees * 9.0 / 5.0 + 32;
}

double transferFunction(Temperature* t, int rawVal) {
    return 0.0; // TODO: Implement
}
