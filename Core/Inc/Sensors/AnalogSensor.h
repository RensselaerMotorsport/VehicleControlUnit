#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H

#ifndef TEST_MODE
// STM32 HAL includes
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_uart.h"
#endif

// Standard C includes
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// User includes
#include "Sensor.h"

// Structures
typedef struct {
    Sensor sensor;
    int channel;
    void* child; // Pointer to the child struct
} AnalogSensor;

// Function declarations

/**
 * @brief Initializes an analog sensor and configures the corresponding GPIO pin
 *
 * @param sensor Pointer to the AnalogSensor structure to initialize
 * @param name Name of the sensor (string)
 * @param hz Sampling frequency in Hertz
 * @param channel ADC channel number for the sensor (0-16)
 *
 * This function initializes the base sensor properties, sets the ADC channel,
 * and configures the corresponding GPIO pin based on the channel number.
 */
void initAnalogSensor(AnalogSensor* analogSensor, const char* name, int hz, int channel, void* child);

/**
 * @brief Processes ADC data from all three ADCs and stores it in the circular buffer
 *
 * @param adc1_data Pointer to ADC1 data buffer (channels 0-5)
 * @param adc2_data Pointer to ADC2 data buffer (channels 6-11)
 * @param adc3_data Pointer to ADC3 data buffer (channels 12-15)
 *
 * This function combines data from all three ADCs into a single ADCSample
 * and adds it to the circular buffer. It also sends debug information via UART.
 */
void ProcessADCData(uint32_t* adc1_buffer, uint32_t* adc2_buffer, uint32_t* adc3_buffer);

/**
 * @brief Retrieves analog sensor data for a specific channel
 *
 * @param sensor Pointer to the AnalogSensor structure
 * @return int The current ADC value for the specified channel
 *
 * This function returns the latest ADC value for the channel specified in the AnalogSensor structure.
 * If an invalid channel is specified, it returns 0.
 */
float getAnalogSensorData(AnalogSensor* sensor);

#ifndef TEST_MODE
// External variable declarations
extern ADC_HandleTypeDef hadc1, hadc2, hadc3;
extern DMA_HandleTypeDef hdma_adc1, hdma_adc2, hdma_adc3;
#endif

// ADC buffers
extern uint32_t adc1_buffer[7];
extern uint32_t adc2_buffer[7];
extern uint32_t adc3_buffer[8];

#endif // RENSSELAERMOTORSPORT_ANALOGSENSOR_H

