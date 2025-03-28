#ifndef RENNSSELAERMOTORSPORT_ANALOGOUTPUT_H
#define RENNSSELAERMOTORSPORT_ANALOGOUTPUT_H

#include "Output.h"
#include "../Utils/Constants.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    Output output;
    int channel;
    int buffer_index;
} AnalogOutput;

extern uint32_t dac1_buffer[DAC1_BUFFER_SIZE];
extern uint32_t dac2_buffer[DAC2_BUFFER_SIZE];

/**
 * @brief Initializes an analog output
 *
 * @param output Pointer to the AnalogOutput structure to initialize
 * @param name Name of the output (string)
 * @param hz Sampling frequency in Hertz
 * @param channel ADC channel number for the output (0-16)
 *
 * This function initializes the base output properties, sets the ADC channel,
 * and configures the corresponding GPIO pin based on the channel number.
 */
void initAnalogOutput(AnalogOutput* analogoutput, const char* name, int hz,
                       int channel);

/**
 * @brief Writes data to the analog output.
 *  
 * @param output Pointer to the AnalogOutput structure.
 * @param data Data to write to the output.
 * @return int _SUCCESS or _FAILURE.
 */
int writeAnalogOutputData(AnalogOutput* output, float data);

#endif // RENNSSELAERMOTORSPORT_ANALOGOUTPUT_H
