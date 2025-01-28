#include "../../Inc/Utils/Logger.h"

// Add a log entry to the buffer
void logToBuffer(const char* logEntry) {
    if (logCount >= LOG_BUFFER_SIZE) {
        // Dump the buffer and reset if full
        printf("Log buffer full. Dumping logs to prevent overwriting.\n");
        flushLogsToFile("log.csv");
        logHead = 0;
        logTail = 0;
        logCount = 0;
    }

    // Add the log entry to the buffer
    strncpy(logBuffer[logHead], logEntry, LOG_ENTRY_SIZE - 1);
    logBuffer[logHead][LOG_ENTRY_SIZE - 1] = '\0';
    logHead = (logHead + 1) % LOG_BUFFER_SIZE;
    logCount++;
}

// Flush the log buffer to a file
void flushLogsToFile(const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Error: Unable to open log file: %s\n", filename);
        return;
    }

    while (logCount > 0) {
        fprintf(file, "%s\n", logBuffer[logTail]);
        logTail = (logTail + 1) % LOG_BUFFER_SIZE;
        logCount--;
    }

    fclose(file);
}
