/**
  ******************************************************************************
  * @file           : SDLogger.h
  * @brief          : Logs data in CSV format to SD card.
  ******************************************************************************
*/

#ifndef RENSSELAERMOTORSPORT_SDLOGGER_H
#define RENSSELAERMOTORSPORT_SDLOGGER_H

#include <stdint.h>

/**
 * @brief Start the SD logger task (mounts card, opens log file).
 * Creates the low-priority logger task which will mount the SD card,
 * open the CSV log file, and begin draining the internal queue.
 */
void initSdLogging();

/**
 * @brief Register a channel's metadata for later SD logging.
 * This only stores it in RAM until the SD is mounted.
 *
 * @param name   Channel name
 * @param unit   Unit string
 * @param min_v  Minimum expected value
 * @param max_v  Maximum expected value
 */
void sdRegisterChannel(const char* name, const char* unit, float min_v, float max_v);

/**
 * @brief Queue a value for logging to CSV.
 * Non-blocking. Can be called from high-rate tasks. If the logger task
 * is not ready or the queue is full, the sample is dropped.
 *
 * @param name  signal name
 * @param value signal value
 */
void sdLogValue(const char *name, float value);

#endif // RENSSELAERMOTORSPORT_SDLOGGER_H
