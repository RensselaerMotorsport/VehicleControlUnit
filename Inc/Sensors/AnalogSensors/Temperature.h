#ifndef RENSSELAERMOTORSPORT_TEMPERATURE_H
#define RENSSELAERMOTORSPORT_TEMPERATURE_H

#include "../AnalogSensor.h"

typedef struct {
    AnalogSensor base;
    double degrees;
} Temperature;

/**
 * @brief Initializes the Temperature sensor with the given frequency and channel.
 *
 * @param temp    Pointer to the Temperature structure to initialize.
 * @param hz      The frequency in Hertz at which the sensor operates.
 * @param channel The analog channel the sensor is connected to.
 */
void initTemperature(Temperature* temp, int hz, int channel);

/**
 * @brief Updates the Temperature data.
 *
 * @param temp Pointer to the Temperature structure to update.
 */
void updateTemperature(void* temp);

/**
 * @brief Gets the current temperature in Celsius.
 *
 * @param temp Pointer to the Temperature structure to query.
 * @return     The current temperature in Celsius.
 */
double getTemperatureCelsius(Temperature* temp);

/**
 * @brief Gets the current temperature in Fahrenheit.
 *
 * @param temp Pointer to the Temperature structure to query.
 * @return     The current temperature in Fahrenheit.
 */
double getTemperatureFahrenheit(Temperature* temp);

/**
 * @brief Converts raw temperature data to a meaningful temperature value.
 *
 * @param temp   Pointer to the Temperature structure.
 * @param rawVal The raw data from the temperature sensor.
 * @return       The converted temperature value.
 */
double transferFunction(Temperature* temp, int rawValue);

#endif // RENSSELAERMOTORSPORT_TEMPERATURE_H
