#include "../../Inc/Sensors/AnalogSensor.h"
#include <stdio.h>

/**
 * @brief Initializes an analog sensor and configures the corresponding GPIO pin
 *
 * @param sensor Pointer to the AnalogSensor structure to initialize
 * @param name Name of the sensor (string)
 * @param hz Sampling frequency in Hertz
 * @param channel ADC channel number for the sensor (0-16)
 *
 * This function initializes the base sensor properties, sets the ADC channel,
 * and configures the corresponding GPIO pin based on the channel number:
 * - Channels 0-7: GPIOA
 * - Channels 8-9: GPIOB
 * - Channels 10-16: GPIOC
 */

void initAnalogSensor(AnalogSensor* sensor, const char* name, int hz, int channel) {
    initSensor(&sensor->base, name, hz);
    sensor->channel = channel;

	// Struct for intializing the common settings for all GPIO channels
    GPIO_InitTypeDef GPIOXout_Struct;
    GPIOXout_Struct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOXout_Struct.Pull = GPIO_PULLUP;
    GPIOXout_Struct.Speed = GPIO_SPEED_MEDIUM;

	// Condition checks the value of sensor->channel
	// Channels 0-7 -> GPIOA, 8-9 -> GPIOB, 10-16 -> GPIOC
    if (sensor->channel >= 0 && sensor->channel <= 7) {
        __HAL_RCC_GPIOA_CLK_ENABLE();

        switch (sensor->channel) {
            case 0:
                // HAL code to bind to PA0
                GPIOXout_Struct.Pin = GPIO_PIN_0;
                break;
            case 1:
                // HAL code to bind to PA1
                GPIOXout_Struct.Pin = GPIO_PIN_1;
                break;
            case 2:
                // HAL code to bind to PA2
                GPIOXout_Struct.Pin = GPIO_PIN_2;
                break;
            case 3:
                // HAL code to bind to PA3
                GPIOXout_Struct.Pin = GPIO_PIN_3;
                break;
            case 4:
                // HAL code to bind to PA4
                GPIOXout_Struct.Pin = GPIO_PIN_4;
                break;
            case 5:
                // HAL code to bind to PA5
                GPIOXout_Struct.Pin = GPIO_PIN_5;
                break;
            case 6:
                // HAL code to bind to PA6
                GPIOXout_Struct.Pin = GPIO_PIN_6;
                break;
            case 7:
                // HAL code to bind to PA7
                GPIOXout_Struct.Pin = GPIO_PIN_7;
                break;
            default:
                // Handle invalid channel numbers
                break;
        }

        HAL_GPIO_Init(GPIOA, &GPIOXout_Struct);

    } else if (sensor->channel >= 8 && sensor->channel <= 9) {
        __HAL_RCC_GPIOB_CLK_ENABLE();

        switch (sensor->channel) {
            case 8:
                // HAL code to bind to PB0
                GPIOXout_Struct.Pin = GPIO_PIN_0;
                break;
            case 9:
                // HAL code to bind to PB1
                GPIOXout_Struct.Pin = GPIO_PIN_1;
                break;
            default:
                // Handle invalid channel numbers
                break;
        }

		HAL_GPIO_Init(GPIOB, &GPIOXout_Struct);

    } else if (sensor->channel >= 10 && sensor->channel <= 16) {
        __HAL_RCC_GPIOC_CLK_ENABLE();

        switch (sensor->channel) {
            case 10:
                // HAL code to bind to PC0
                GPIOXout_Struct.Pin = GPIO_PIN_0;
                break;
            case 11:
                // HAL code to bind to PC1
                GPIOXout_Struct.Pin = GPIO_PIN_1;
                break;
            case 12:
                // HAL code to bind to PC2
                GPIOXout_Struct.Pin = GPIO_PIN_2;
                break;
			case 13:
                // HAL code to bind to PC3
                GPIOXout_Struct.Pin = GPIO_PIN_3;
                break;
			case 14:
                // HAL code to bind to PC4
                GPIOXout_Struct.Pin = GPIO_PIN_4;
                break;
			case 15:
                // HAL code to bind to PC5
                GPIOXout_Struct.Pin = GPIO_PIN_5;
                break;
			case 16:
                // HAL code to bind to PC6
                GPIOXout_Struct.Pin = GPIO_PIN_6;
                break;
            default:
                // Handle invalid channel numbers
                break;
        }

        HAL_GPIO_Init(GPIOC, &GPIOXout_Struct);
    }
}

// TODO: Make a function to retrieve data
int getAnalogSensorData(AnalogSensor* sensor) {
    // Placeholder for sensor data retrieval logic
    // Would normally interface with hardware here
    return 0; // Default return
}