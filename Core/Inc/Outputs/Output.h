#ifndef RENSSELAERMOTORSPORT_OUTPUT_H
#define RENSSELAERMOTORSPORT_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>

#include "../Utils/Updateable.h"

typedef enum {
    o_ANALOG,
    o_DIGITAL,
} OutputType;

typedef struct {
    Updateable updateable;
    OutputType type;
} Output;

/**
 * @brief Initializes an output with the given parameters.
 *
 * @param output Pointer to the Output structure to initialize.
 * @param name   Pointer to a string containing the name of the output.
 * @param hz     The frequency in Hertz at which the output operates.
 * @param type   The type of the output as defined by the SensorType enum.
 */
void initOutput(Output* output, const char* name, int hz, OutputType type);

#endif // RENSSELAERMOTORSPORT_OUTPUT_H
