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
 * @brief Initializes a analog output with the given parameters.
 *
 * @param analogoutput Pointer to the Output structure to initialize.
 * @param name   Pointer to a string containing the name of the output.
 * @param hz     The frequency in Hertz at which the output operates.
 * @param port   Location of the analog output on board.
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
