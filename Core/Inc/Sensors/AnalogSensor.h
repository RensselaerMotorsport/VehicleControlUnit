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

// Defines
#define ADC_CHANNELS 16  // Total number of ADC channels
#define UART_BUF_SIZE 100
#define BUFFER_SIZE 1000 // Size of the circular buffer, change in the future if needed

// Structures
typedef struct {
    Sensor sensor;
    int channel;
} AnalogSensor;

typedef struct {
    uint16_t adc[ADC_CHANNELS]; // Array to hold all 16 ADC values
} ADCSample;

// Function declarations
void initAnalogSensor(AnalogSensor* analogSensor, const char* name, int hz, int channel);
void ProcessADCData(uint16_t* adc1_data, uint16_t* adc2_data, uint16_t* adc3_data);
ADCSample getLatestSample(void);
uint32_t getRecentSamples(ADCSample* samples, uint32_t num_samples);
int getAnalogSensorData(AnalogSensor* sensor);

#ifndef TEST_MODE
    // External variable declarations
    extern ADC_HandleTypeDef hadc1, hadc2, hadc3;
    extern DMA_HandleTypeDef hdma_adc1, hdma_adc2, hdma_adc3;
    extern UART_HandleTypeDef huart2;
#endif

// ADC buffers
extern uint16_t adc1_buffer[6];
extern uint16_t adc2_buffer[6];
extern uint16_t adc3_buffer[4];

extern char uart_buf[UART_BUF_SIZE];

#endif // RENSSELAERMOTORSPORT_ANALOGSENSOR_H

