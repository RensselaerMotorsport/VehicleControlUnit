#include "Temperature.h"

void InitTemp(Temperature t, int channel, int hz, double degrees){
    t.channel = channel;
    t.hz = hz;
    t.degrees = degrees;
}

double getCelsius(Temperature t){
    return t.degrees;
}
double getFahrenheit(Temperature t){
    return t.degrees * (9.0/5.0) + 32;
}

double TransferFunction(Temperature t, int rawVal) {
    return 0.0; // TODO: Implement
}