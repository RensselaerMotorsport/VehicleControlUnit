#ifndef RENNSSELAERMOTORSPORT_DIGITALOUTPUT_H
#define RENNSSELAERMOTORSPORT_DIGITALOUTPUT_H

#include "Output.h"
#include "../Utils/Constants.h"

#include <stdint.h>

extern uint8_t digital_out_buffer[NUM_DIGITAL_OUTPUTS];

typedef struct {
  Output output;
    int port;
} DigitalOutput;

/**
 * @brief Initializes a digital output with the given parameters.
 *
 * @param digitaloutput Pointer to the Output structure to initialize.
 * @param name   Pointer to a string containing the name of the output.
 * @param hz     The frequency in Hertz at which the output operates.
 * @param port   Location of the digital output on board.
 */
void initDigitalOutput(DigitalOutput* digitaloutput, const char* name, int hz,
                       int port);

/**
 * @brief Writes data to the buffer of the digital output.
 * 
 * @param output Pointer to the DigitalOutput structure.
 * @param data Data to write to the output.
 * @return int _SUCCESS or _FAILURE.
 */
int writeDigitalOutputData(DigitalOutput* output, int data);

#endif // RENNSSELAERMOTORSPORT_DIGITALOUTPUT_H
