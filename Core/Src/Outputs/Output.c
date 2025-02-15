#include "../../Inc/Outputs/Output.h"
#include "../../Inc/Utils/Updateable.h"

void initOutput(Output* output, const char* name, int hz, OutputType type) {
    initUpdateable(&output->updateable, name, hz, OUTPUT, output);
    output->type = type;
}
