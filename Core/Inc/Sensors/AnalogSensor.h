#ifndef RENSSELAERMOTORSPORT_ANALOGSENSOR_H
#define RENSSELAERMOTORSPORT_ANALOGSENSOR_H
// STM32 HAL includes
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_uart.h"

// Standard C includes
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// User includes
#include "Sensor.h"


#define ADC_BUF_LEN 4096
#define ADC_CHANNELS 2
#define UART_BUF_SIZE 100
#define BUFFER_SIZE 1000  // Size of the circular buffer

typedef struct {
    Sensor sensor;
    int channel;
} AnalogSensor;

typedef struct {
    uint16_t adc4;
    uint16_t adc5;
} ADCSample;

/**
 * @brief Initializes an analog sensor.
 *
 * @param analogSensor Pointer to the analog sensor to initialize.
 * @param name Name of the sensor.
 * @param hz Sampling frequency in Hertz.
 * @param channel Analog channel number.
 */

// Function declarations
void initAnalogSensor(AnalogSensor* analogsensor, const char* name, int hz, int channel);
void ProcessADCData(uint16_t* data, uint32_t length);
ADCSample getLatestSample(void);
uint32_t getRecentSamples(ADCSample* samples, uint32_t num_samples);
int getAnalogSensorData(AnalogSensor* sensor);

// External variable declarations
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern UART_HandleTypeDef huart2;

extern uint16_t adc_buf[ADC_BUF_LEN * ADC_CHANNELS];
extern volatile uint16_t adc_value_ch4;
extern volatile uint16_t adc_value_ch5;
extern char uart_buf[UART_BUF_SIZE];

#endif // RENSSELAERMOTORSPORT_ANALOGSENSOR_H

