#include "../../Inc/Outputs/DigitalOutput.h"
#include "../../Inc/Utils/Common.h"
#include "../../Inc/Utils/MessageFormat.h"
#include "../../Inc/Utils/Telemetry.h" 

void initDigitalOutput(DigitalOutput* digitalOutput, const char* name, int hz,
                       int port) {
    if (port < 0 || port >= NUM_DIGITAL_OUTPUTS) {
        sendMessage(name, MSG_ERROR, "Port %u Invalid", port);
        return;
    }
    initOutput(&digitalOutput->output, name, hz, o_DIGITAL);
    digitalOutput->port = port;
    digitalOutput->telem_state = registerTelemetrySignal(name, TELEMETRY_OUTPUT, UNIT_BOOLEAN, 1000/hz, 0, 1);
}

int writeDigitalOutputData(DigitalOutput* output, int data) {
    digital_out_buffer[output->port] = data;
    sendTelemetryValue(output->telem_state, data);
    return _SUCCESS;
}
