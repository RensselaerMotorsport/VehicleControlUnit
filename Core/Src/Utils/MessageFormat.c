#include "../../Inc/Utils/MessageFormat.h"
#include <stdio.h>
#include <stdarg.h>

void sendMessage(const char* sender, MessageType type, const char* format, ...) {
    const char* type_strings[] = {
        "SENSOR_VALUE",
        "OUTPUT_VALUE",
        "SYSTEM_STATUS", 
        "WARNING",
        "ERROR",
        "DEBUG",
        "CAN_TX",
        "CAN_RX",
        "TIMER_STATS",
        "CONFIG"
    };
    
    printf("Sender:%s;InfoType:%s;Content:", sender, type_strings[type]);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\r\n");
}