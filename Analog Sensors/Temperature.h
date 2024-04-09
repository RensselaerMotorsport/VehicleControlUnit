
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

struct Temperature {

    //Standard Information
    int channel;
    int hz;

    double degrees;
    //Celcius

} typedef Temperature;

void InitTemp(Temperature t, int channel, int hz, double degrees);

double TransferFunction(Temperature t, int rawVal);

double getCelsius(Temperature t);

double getFahrenheit(Temperature t);

#endif