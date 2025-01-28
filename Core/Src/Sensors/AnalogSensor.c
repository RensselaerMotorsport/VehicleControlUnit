#include "../../Inc/Sensors/AnalogSensor.h"

#include <stdio.h>
#include <string.h>

#ifndef TEST_MODE
#include "stm32f7xx_hal.h"
#endif


// Circular buffer to store ADC samples
static ADCSample adc_circular_buffer[BUFFER_SIZE];
static uint32_t buffer_head = 0;
static uint32_t buffer_tail = 0;
static uint32_t buffer_count = 0;

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
        /*HAL_GPIO_Init(GPIOA, &GPIOXout_Struct);*/ // Commented out due to not being recognized
    } else if (channel == 6 || channel == 7) {
        // ADC2: PA6-PA7
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_6 << (channel - 6);
        /*HAL_GPIO_Init(GPIOA, &GPIOXout_Struct);*/
    } else if (channel == 8 || channel == 9) {
        // ADC2: PB0-PB1
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_0 << (channel - 8);
        /*HAL_GPIO_Init(GPIOB, &GPIOXout_Struct);*/
    } else if (channel >= 10 && channel <= 13) {
        // ADC3: PC0-PC3
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_0 << (channel - 10);
        /*HAL_GPIO_Init(GPIOC, &GPIOXout_Struct);*/
    } else if (channel == 14 || channel == 15) {
        // ADC2: PC4-PC5
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIOXout_Struct.Pin = GPIO_PIN_4 << (channel - 14);
        /*HAL_GPIO_Init(GPIOC, &GPIOXout_Struct);*/
    }
    #endif
}

/**
 * @brief Adds a sample to the circular buffer
 *
 * @param sample ADCSample to be added to the buffer
 *
 * This function adds a new sample to the circular buffer. If the buffer is full,
 * it overwrites the oldest data.
 */
static void addSampleToBuffer(ADCSample sample) {
    adc_circular_buffer[buffer_head] = sample;
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    if (buffer_count < BUFFER_SIZE) {
        buffer_count++;
    } else {
        // Buffer is full, update tail to overwrite the oldest data
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    }
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
void ProcessADCData(uint16_t* adc1_data, uint16_t* adc2_data, uint16_t* adc3_data) {
    ADCSample sample = {0};  // Initialize all channels to 0

    // Process ADC1 data (PA0-PA5: channels 0-5)
    for (int i = 0; i < 6; i++) {
        sample.adc[i] = adc1_data[i];
    }

    // Process ADC2 data (PA6-PA7: channels 6-7, PB0-PB1: channels 8-9, PC4-PC5: channels 14-15)
    sample.adc[6] = adc2_data[0];  // PA6
    sample.adc[7] = adc2_data[1];  // PA7
    sample.adc[8] = adc2_data[2];  // PB0
    sample.adc[9] = adc2_data[3];  // PB1
    sample.adc[14] = adc2_data[4]; // PC4
    sample.adc[15] = adc2_data[5]; // PC5

    // Process ADC3 data (PC0-PC3: channels 10-13)
    for (int i = 0; i < 4; i++) {
        sample.adc[i + 10] = adc3_data[i];
    }

    addSampleToBuffer(sample);

    // Optional: UART debug output (adjust as needed)
    char uart_buf[100];
    snprintf(uart_buf, sizeof(uart_buf), "ADC0: %4d, ADC7: %4d, ADC10: %4d\r\n",
             sample.adc[0], sample.adc[7], sample.adc[10]);
    #ifndef TEST_MODE
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    #endif

}


/**
 * @brief Retrieves the latest ADC sample from the buffer
 *
 * @return ADCSample The most recent ADC sample added to the buffer
 *
 * If the buffer is empty, this function returns a sample with all channels set to 0.
 */
ADCSample getLatestSample(void) {
    if (buffer_count > 0) {
        uint32_t latest = (buffer_head - 1 + BUFFER_SIZE) % BUFFER_SIZE;
        return adc_circular_buffer[latest];
    }

    // Return a default sample if buffer is empty
	ADCSample empty_sample = {0};
	return empty_sample;
}

/**
 * @brief Retrieves a specified number of recent samples from the buffer
 *
 * @param samples Pointer to an array to store the retrieved samples
 * @param num_samples Number of samples to retrieve
 * @return uint32_t Actual number of samples retrieved
 *
 * This function copies the most recent samples from the circular buffer to the provided array.
 * It returns the actual number of samples copied, which may be less than requested if the buffer is not full.
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
 * @brief Retrieves analog sensor data for a specific channel
 *
 * @param sensor Pointer to the AnalogSensor structure
 * @return int The current ADC value for the specified channel
 *
 * This function returns the latest ADC value for the channel specified in the AnalogSensor structure.
 * If an invalid channel is specified, it returns 0.
 */
int getAnalogSensorData(AnalogSensor* sensor) {
    ADCSample latest = getLatestSample();
    if (sensor->channel >= 0 && sensor->channel < 16) {
        return latest.adc[sensor->channel];
    }
    return 0; // Default return for invalid channels
}
