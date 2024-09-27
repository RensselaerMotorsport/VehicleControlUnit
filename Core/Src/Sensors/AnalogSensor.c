#include "../../Inc/Sensors/AnalogSensor.h"
#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"

// Circular buffer to store ADC samples
static ADCSample adc_circular_buffer[BUFFER_SIZE];
static uint32_t buffer_head = 0;
static uint32_t buffer_tail = 0;
static uint32_t buffer_count = 0;

// The getAnalogSensorData function only handles channels 4 and 5,
// but initAnalogSensor sets up channels 0-16.
// AnalogSensorData can be expanded to handle all possible channels,
// or adjust initAnalogSensor if only 2 channels (4 and 5) is needed.

// Initialization code in main.c would set up the ADC to continuously sample channels 4 and 5,
// and use DMA to transfer the data to adc_buf.


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

void initAnalogSensor(AnalogSensor* analogsensor, const char* name, int hz, int channel) {
    initSensor(&analogsensor->sensor, name, hz, ANALOG);
    analogsensor->channel = channel;

    GPIO_InitTypeDef GPIOXout_Struct = {0};
    GPIOXout_Struct.Mode = GPIO_MODE_ANALOG;
    GPIOXout_Struct.Pull = GPIO_NOPULL;
    GPIOXout_Struct.Speed = GPIO_SPEED_FREQ_HIGH;

	// Condition checks the value of sensor->channel
	// Channels 0-7 -> GPIOA, 8-9 -> GPIOB, 10-16 -> GPIOC
    if (analogsensor->channel >= 0 && analogsensor->channel <= 7) {
        __HAL_RCC_GPIOA_CLK_ENABLE();

        switch (analogsensor->channel) {
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

    } else if (analogsensor->channel >= 8 && analogsensor->channel <= 9) {
        __HAL_RCC_GPIOB_CLK_ENABLE();

        switch (analogsensor->channel) {
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

    } else if (analogsensor->channel >= 10 && analogsensor->channel <= 16) {
        __HAL_RCC_GPIOC_CLK_ENABLE();

        switch (analogsensor->channel) {
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

/**
 * @brief Add a sample to the circular buffer
 * @param sample ADCSample to be added
 */
static void addSampleToBuffer(ADCSample sample) {
    if (buffer_count < BUFFER_SIZE) {
        adc_circular_buffer[buffer_head] = sample;
        buffer_head = (buffer_head + 1) % BUFFER_SIZE;
        buffer_count++;
    } else {
        // Buffer is full, overwrite the oldest data
        adc_circular_buffer[buffer_head] = sample;
        buffer_head = (buffer_head + 1) % BUFFER_SIZE;
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    }
}

/**
 * @brief Process ADC data for channels 4 and 5, and store in circular buffer
 * @param data Pointer to the ADC data buffer
 * @param length Length of the data buffer
 */
void ProcessADCData(uint16_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i += 2) {
        // Store ADC values for channels 4 and 5
        ADCSample sample = {
            .adc4 = data[i],
            .adc5 = data[i + 1]
        };

        // Add sample to circular buffer
        addSampleToBuffer(sample);

        // Optionally, you can still send UART messages for debugging
        snprintf(uart_buf, UART_BUF_SIZE, "ADC4: %4d, ADC5: %4d\r\n", sample.adc4, sample.adc5);
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    }
}

/**
 * @brief Get the latest ADC sample from the buffer
 * @return ADCSample The latest ADC sample
 */
ADCSample getLatestSample(void) {
    if (buffer_count > 0) {
        uint32_t latest = (buffer_head - 1 + BUFFER_SIZE) % BUFFER_SIZE;
        return adc_circular_buffer[latest];
    } else {
        // Return a default sample if buffer is empty
        return (ADCSample){0, 0};
    }
}

/**
 * @brief Get a specific number of recent samples from the buffer
 * @param samples Pointer to an array to store the samples
 * @param num_samples Number of samples to retrieve
 * @return uint32_t Actual number of samples retrieved
 */
uint32_t getRecentSamples(ADCSample* samples, uint32_t num_samples) {
    uint32_t samples_to_copy = (num_samples < buffer_count) ? num_samples : buffer_count;

    for (uint32_t i = 0; i < samples_to_copy; i++) {
        uint32_t index = (buffer_head - 1 - i + BUFFER_SIZE) % BUFFER_SIZE;
        samples[i] = adc_circular_buffer[index];
    }

    return samples_to_copy;
}

/**
 * @brief Get analog sensor data for a specific channel
 * @param sensor Pointer to the AnalogSensor structure
 * @return int The current ADC value for the specified channel
 */
int getAnalogSensorData(AnalogSensor* sensor) {
    ADCSample latest = getLatestSample();
    if (sensor->channel == 4) {
        return latest.adc4;
    } else if (sensor->channel == 5) {
        return latest.adc5;
    }
    return 0; // Default return for other channels
}
