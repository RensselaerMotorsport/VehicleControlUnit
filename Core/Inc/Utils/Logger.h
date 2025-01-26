#ifndef RENSSELAERMOTORSPORT_LOGGER_H
#define RENSSELAERMOTORSPORT_LOGGER_H

#include <stdio.h>
#include <string.h>

// Macro for logging
#define LOG(item_) ((item_)->updateable->log((item_)->updateable->context))

// Buffer size configuration
#define LOG_BUFFER_SIZE 1024
#define LOG_ENTRY_SIZE  64 // Size of each log entry

// Log buffer structure
static char logBuffer[LOG_BUFFER_SIZE][LOG_ENTRY_SIZE];
static int logHead = 0;  // Points to the next empty slot
static int logTail = 0;  // Points to the oldest log entry
static int logCount = 0; // Number of entries in the buffer

// TODO: Add comments
void logToBuffer(const char* logEntry);
void flushLogsToFile(const char* filename);

#endif // RENSSELAERMOTORSPORT_LOGGER_H
