#include "../../../includes/Temperature.h"

void initTemperature(Temperature* temp, int hz) {
    initAnalogSensor(&temp->base, "Temperature", hz);
    temp->base.base.update = updateTemperature;
    temp->temp = 0;
}

void updateTemperature(void* temp) {
    Temperature *myTemp = (Temperature *)temp;
    // Placeholder for update logic
    printf("Implement update logic for Temperature Sensor\n");
}

double getTemperatureCelsius(Temperature* temp) {
    return temp->temp;
}

double getTemperatureFahrenheit(Temperature* temp) {
    return temp->temp * 9.0 / 5.0 + 32;
}

double transferFunction(Temperature* t, int rawVal) {
    return 0.0; // TODO: Implement
}
