#include "../../Inc/Outputs/DigitalOutput.h"
#include "../../Inc/Utils/Common.h"

void initDigitalOutput(DigitalOutput* digitalOutput, const char* name, int hz,
                       int port) {
    if (port < 0 || port >= NUM_DIGITAL_OUTPUTS) {
        
        #ifdef DEBUGn
        printf("Invalid port number for digital output %s\n", name);
        #endif

        return;
    }
    initOutput(&digitalOutput->output, name, hz, o_DIGITAL);
    digitalOutput->port = port;
}

int writeDigitalOutputData(DigitalOutput* output, int data) {
    
    #ifdef DEBUGn
    printf("Writing data to digital output %s: %d\n", output->output.updateable.name, data);
    #endif
    digital_out_buffer[output->port] = data;
    return _SUCCESS;
}
