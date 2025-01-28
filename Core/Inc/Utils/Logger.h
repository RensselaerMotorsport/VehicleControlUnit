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

/**
 * @brief Adds a log entry to the circular buffer. If the buffer is full, it
 * dumps the logs to a file.
 *
 * @param logEntry The log entry to add to the buffer.
 * Must be a null-terminated string.
 */
void logToBuffer(const char* logEntry);

/**
 * @brief Flushes the log buffer to a file by appending each log entry to the
 * specified file.
 *
 * @param filename The name of the file to which the logs will be written.
 * Must be a null-terminated string.
 */
void flushLogsToFile(const char* filename);

#endif // RENSSELAERMOTORSPORT_LOGGER_H
