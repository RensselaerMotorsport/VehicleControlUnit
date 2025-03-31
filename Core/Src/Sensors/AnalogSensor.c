#include "../../Inc/Sensors/AnalogSensor.h"
#include "../../Inc/Utils/Constants.h"

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
    initSensor(&analogSensor->sensor, name, hz, s_ANALOG);
    analogSensor->channel = channel;
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
void ProcessADCData(uint32_t* adc1_buffer, uint32_t* adc2_buffer, uint32_t* adc3_buffer) {

    // Channels 0, 2, 6, 8, 19, 12, 14 on ADC1
    for (int i = 0; i < ADC1_CHANNEL_SIZE; i++) {
        adc_samples[i] = adc1_buffer[i];
    }

    // Channels 1, 3, 7, 9, 10, 13, 15 on ADC2
    for (int i = ADC1_CHANNEL_SIZE; i < ADC1_CHANNEL_SIZE+ADC2_CHANNEL_SIZE; i++) {
        adc_samples[i] = adc2_buffer[i-7];
    }

    // Channels 4, 5, 6, 7, 8, 9, 14, 15 on ADC3
    for (int i = ADC1_CHANNEL_SIZE+ADC2_CHANNEL_SIZE; i < ADC_CHANNELS; i++) {
        adc_samples[i] = adc3_buffer[i-14];
    }

//    printf("8: %10d, 4: %10d, 12: %10d, 19: %10d, 21: %10d, 18: %10d, 10: %10d\r\n, 5: %10d\r\n, 6: %10d\r\n",
//           adc_samples[8], adc_samples[4], adc_samples[12], adc_samples[19], adc_samples[21], adc_samples[18], adc_samples[10], adc_samples[5], adc_samples[6]);

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
    if (sensor->channel >= 0 && sensor->channel < ADC_CHANNELS) {
        return ((float)adc_samples[sensor->channel]/4096)*ADC_VREF;
    }
    printf("Invalid channel specified for AnalogSensor %s\r\n", sensor->sensor.updateable.name);
    return 0; // Default return for invalid channels
}
