#include "../../Inc/Sensors/AnalogSensor.h"

#include <stdio.h>
#include <string.h>

#ifndef TEST_MODE
#include "stm32f7xx_hal.h"
#endif


// Circular buffer to store ADC samples
static uint16_t adc_samples[ADC_CHANNELS];

// The AnalogSensor system is configured to handle 16 channels (0-15) across three ADCs:
// - ADC1: Channels 0-5 (PA0-PA5)
// - ADC2: Channels 6-7 (PA6-PA7), 8-9 (PB0-PB1), 14-15 (PC4-PC5)
// - ADC3: Channels 10-13 (PC0-PC3)

// The initAnalogSensor function sets up GPIO pins for all these channels.
// The ProcessADCData function handles data from all three ADCs.
// The getAnalogSensorData function can retrieve data for any channel (0-15).

// Initialization code in main.c should set up the ADCs to continuously sample
// their respective channels and use DMA to transfer the data to the appropriate buffers:
// - adc1_buffer for ADC1 (6 channels)
// - adc2_buffer for ADC2 (6 channels)
// - adc3_buffer for ADC3 (4 channels)


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
void initAnalogSensor(AnalogSensor* analogSensor, const char* name, int hz, int channel) {
    initSensor(&analogSensor->sensor, name, hz, ANALOG);
    analogSensor->channel = channel;

    #ifndef TEST_MODE
    GPIO_InitTypeDef GPIOXout_Struct = {0};
    GPIOXout_Struct.Mode = GPIO_MODE_ANALOG;
    GPIOXout_Struct.Pull = GPIO_NOPULL;
    GPIOXout_Struct.Speed = GPIO_SPEED_FREQ_HIGH;

    // Map channels to appropriate GPIO pins based on ADC configuration
    if (channel >= 0 && channel <= 5) {
        // ADC1: PA0-PA5
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_0 << channel;
        HAL_GPIO_Init(GPIOA, &GPIOXout_Struct); // Commented out due to not being recognized
    } else if (channel == 6 || channel == 7) {
        // ADC2: PA6-PA7
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_6 << (channel - 6);
        HAL_GPIO_Init(GPIOA, &GPIOXout_Struct);
    } else if (channel == 8 || channel == 9) {
        // ADC2: PB0-PB1
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_0 << (channel - 8);
        HAL_GPIO_Init(GPIOB, &GPIOXout_Struct);
    } else if (channel >= 10 && channel <= 13) {
        // ADC3: PC0-PC3
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_0 << (channel - 10);
        HAL_GPIO_Init(GPIOC, &GPIOXout_Struct);
    } else if (channel == 14 || channel == 15) {
        // ADC2: PC4-PC5
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_4 << (channel - 14);
        HAL_GPIO_Init(GPIOC, &GPIOXout_Struct);
    }
    #endif
}

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
void ProcessADCData(uint16_t* adc1_buffer, uint16_t* adc2_buffer, uint16_t* adc3_buffer) {
    // Process ADC1 data (PA0-PA5: channels 0-5)
    for (int i = 0; i < 6; i++) {
        adc_samples[i] = adc1_buffer[i];
    }

    // Process ADC2 data (PA6-PA7: channels 6-7, PB0-PB1: channels 8-9, PC4-PC5: channels 14-15)
    adc_samples[6] = adc2_buffer[0];  // PA6
    adc_samples[7] = adc2_buffer[1];  // PA7
    adc_samples[8] = adc2_buffer[2];  // PB0
    adc_samples[9] = adc2_buffer[3];  // PB1
    adc_samples[14] = adc2_buffer[4]; // PC4
    adc_samples[15] = adc2_buffer[5]; // PC5

    // Process ADC3 data (PC0-PC3: channels 10-13)
    for (int i = 0; i < 4; i++) {
        adc_samples[i + 10] = adc3_buffer[i];
    }

    // printf("0: %10d, 1: %10d, 2: %10d, 3: %10d, 4: %10d, 5: %10d, 6: %10d, 7: %10d \r\n",
    //        adc_samples.adc[0], adc_samples.adc[1], adc_samples.adc[2], adc_samples.adc[3], adc_samples.adc[4], adc_samples.adc[5],
    //        adc_samples.adc[6], adc_samples.adc[10]);

}

/**
 * @brief Retrieves analog sensor data for a specific channel
 *
 * @param sensor Pointer to the AnalogSensor structure
 * @return int The current ADC value for the specified channel
 *
 * This function returns the latest ADC value for the channel specified in the AnalogSensor structure.
 * If an invalid channel is specified, it returns 0.
 */
float getAnalogSensorData(AnalogSensor* sensor) {
    if (sensor->channel >= 0 && sensor->channel < 16) {
        return ((float)adc_samples[sensor->channel]/4096)*VREF;
    }
    printf("Invalid channel specified for AnalogSensor %s\r\n", sensor->sensor.updateable.name);
    return 0; // Default return for invalid channels
}
