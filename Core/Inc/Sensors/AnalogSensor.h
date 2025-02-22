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

// STM32 HAL includes
#include "../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f7xx.h"
#include "../../../Drivers/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal.h"
#include "../../../Drivers/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal_adc.h"
#include "../../../Drivers/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal_dma.h"
#include "../../../Drivers/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal_uart.h"

// User includes
#include "Sensor.h"

// Structures
typedef struct {
    Sensor sensor;
    int channel;
} AnalogSensor;

// Function declarations
void initAnalogSensor(AnalogSensor* analogSensor, const char* name, int hz, int channel);
void ProcessADCData(uint32_t* adc1_buffer, uint32_t* adc2_buffer, uint32_t* adc3_buffer);
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

