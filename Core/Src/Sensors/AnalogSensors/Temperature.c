#include "../../../Inc/Sensors/AnalogSensors/Temperature.h"

void initTemperature(Temperature* temp, int hz, int channel) {
    initAnalogSensor(&temp->base, "Temperature", hz, channel);
    temp->base.sensor.updateable.update = updateTemperature;
    temp->base.sensor.updateable.context = temp;
    temp->base.sensor.updateable.log = logTemperature;
    temp->degrees = 0;
    temp->rawData = 0.0f;
}

void updateTemperature(void* temp) {
    printf("Implement Temperature::update\n");
    Temperature *myTemp = (Temperature *)temp;
    myTemp->rawData = 0.0f;;
    myTemp->degrees = transferFunction(temp, 0);
}

void logTemperature(void* temp) {
    Temperature *myTemp = (Temperature *)temp;
    char logEntry[LOG_ENTRY_SIZE];

    snprintf(logEntry, LOG_ENTRY_SIZE, "Brake Temperature, %.2f, %.2f",
             myTemp->rawData, myTemp->degrees);
    logToBuffer(logEntry);
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
