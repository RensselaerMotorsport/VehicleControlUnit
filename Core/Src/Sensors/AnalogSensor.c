#include "../../Inc/Sensors/AnalogSensor.h"
#include <stdio.h>

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz, int channel) {
    initSensor(&sensor->base, name, hz);
    sensor->channel = channel;

    if(sensor->channel == 0) {
    	// HAL code to bind to PA0
    	__HAL_RCC_GPIOA_CLK_ENABLE();
    	GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_0;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

    } else if (sensor->channel == 1) {
		//HAL code to bind to PA1
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_1;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

    } else if (sensor->channel == 2) {
		//HAL code to bind to PA2
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_2;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

    } else if (sensor->channel == 3) {
		//HAL code to bind to PA3
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_3;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

    } else if (sensor->channel == 4) {
    	//HAL code to bind to PA4
    	__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_4;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

	} else if (sensor->channel == 5) {
		//HAL code to bind to PA5
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_5;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

	} else if (sensor->channel == 6) {
		//HAL code to bind to PA6
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_6;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

	} else if (sensor->channel == 7) {
		//HAL code to bind to PA7
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIOAout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_7;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIOAout_Struct);

	} else if (sensor->channel == 8) {
		//HAL code to bind to PBO
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitTypeDef GPIOBout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_0;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOB, &GPIOBout_Struct);

	} else if (sensor->channel == 9) {
		//HAL code to bind to PB1
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitTypeDef GPIOBout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_1;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOB, &GPIOBout_Struct);

	} else if (sensor->channel == 10) {
		//HAL code to bind to PC0
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_0;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 11) {
		//HAL code to bind to PC1
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_1;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 12) {
		//HAL code to bind to PC2
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_2;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 13) {
		//HAL code to bind to PC3
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_3;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 14) {
		//HAL code to bind to PC4
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_4;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 15) {
		//HAL code to bind to PC5
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_5;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);

	} else if (sensor->channel == 16) {
		//HAL code to bind to PC6
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitTypeDef GPIOCout_Struct;
		GPIOJout_Struct.Pin = GPIO_PIN_6;
		GPIOJout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIOJout_Struct.Pull = GPIO_PULLUP;
		GPIOJout_Struct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIOCout_Struct);
	}


}

// TODO: Make a function to retrieve data
int getAnalogSensorData(AnalogSensor* sensor) {
    // Placeholder for sensor data retrieval logic
    // Would normally interface with hardware here
    return 0; // Default return
}
