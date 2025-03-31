#include "../../Inc/Outputs/AnalogOutput.h"
#include "../../Inc/Utils/Common.h"

#include <stdio.h>
#include <string.h>

#ifndef TEST_MODE
#include "stm32f7xx_hal.h"
#endif

void initAnalogOutput(AnalogOutput* analogOutput, const char* name, int hz, int channel) {
    initOutput(&analogOutput->output, name, hz, o_ANALOG);
    analogOutput->channel = channel;
    analogOutput->buffer_index = 0;
}

int writeAnalogOutputData(AnalogOutput* output, float data) {
    #ifdef DEBUGn
    printf("Writing data to analog output %s: %f\n", output->output.updateable.name, data);
    #endif

    if (output->channel == 1) {
      // Write data to DAC1
      dac1_buffer[output->buffer_index] = (int)data;
      output->buffer_index = (output->buffer_index + 1) % DAC1_BUFFER_SIZE;
    } else if (output->channel == 2) {
      // Write data to DAC2
      dac2_buffer[output->buffer_index] = (int)data;
      output->buffer_index = (output->buffer_index + 1) % DAC2_BUFFER_SIZE;
    } else {
      
      #ifdef DEBUGn
      printf("Invalid channel number for analog output %s\n", output->output.updateable.name);
      #endif

      return _FAILURE;
    }
    return _SUCCESS;
}
