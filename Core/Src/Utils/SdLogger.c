#include "../../Inc/Utils/SdLogger.h"
#include "../../Inc/Utils/SdFunctions.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 128
#define LOG_LINE_MAX 128
#define LOG_QUEUE_LEN 256
#define MAX_LOG_CHANNELS 128
#define LOGGER_TASK_DELAY_MS 1

extern RTC_HandleTypeDef hrtc;

// Logging state
typedef struct {
    char line[LOG_LINE_MAX];
} log_item_t;

typedef struct {
    char name[32];
    char unit[16];
    float min_v;
	float max_v;
} channel_meta_t;

static char current_log_filename[32];
static log_item_t log_queue[LOG_QUEUE_LEN];
static volatile uint16_t log_head = 0;
static volatile uint16_t log_tail = 0;

static channel_meta_t meta[MAX_LOG_CHANNELS];
static uint16_t meta_count = 0;

static volatile int logger_ready = 0;
static volatile uint32_t log_drops = 0;
static FIL log_file; 

// Timestamp helper
static int rtc_iso_ts(char *out, size_t out_sz) {
    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;

    HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN);

    // SubSeconds counts DOWN from SynchPrediv to 0
    uint32_t sp = hrtc.Init.SynchPrediv ? hrtc.Init.SynchPrediv : 255u;
    uint32_t ms = ((sp - t.SubSeconds) * 1000u) / (sp + 1u);

    // d.Year is 0..99 (20YY) on F7 HAL
    int year = 2000 + d.Year;

    return snprintf(out, out_sz, "%04d-%02d-%02dT%02d:%02d:%02d.%03lu",
                    year, d.Month, d.Date, t.Hours, t.Minutes, t.Seconds,
                    (unsigned long)ms);
}

void sdRegisterChannel(const char* name, const char* unit, float min_v, float max_v) {
	// Duplicate Check
	for (uint16_t i = 0; i < meta_count; ++i) {
		if (strncmp(meta[i].name, name, sizeof(meta[i].name)) == 0) {
			// duplicate found - This should not happen
			printf("ERROR: Channel '%s' already registered in Log!\n", name);
			return;
		}
	}

	// Overflow check
	if (meta_count >= MAX_LOG_CHANNELS) {
		printf("ERROR: Channel registry full! (MAX_LOG_CHANNELS=%u)\n", MAX_LOG_CHANNELS);
		return;
	}

	// Store new entry
	channel_meta_t *m = &meta[meta_count++];

	// Copy strings safely and terminate
	strncpy(m->name, name, sizeof(m->name) - 1);
	m->name[sizeof(m->name) - 1] = '\0';

	strncpy(m->unit, unit, sizeof(m->unit) - 1);
	m->unit[sizeof(m->unit) - 1] = '\0';

	// Store bounds
	m->min_v = min_v;
	m->max_v = max_v;
}

// Makes a new file name each startup
static void pickNextLogFile(void) {
    char path[32];
    FILINFO fno;
    FRESULT fr;

    for (uint16_t idx = 0; idx < 10000; ++idx) {
        snprintf(path, sizeof(path), "0:/data/log_%04u.csv", idx);
        fr = f_stat(path, &fno);
        if (fr == FR_NO_FILE) {
            strncpy(current_log_filename, path, sizeof(current_log_filename) - 1);
            current_log_filename[sizeof(current_log_filename) - 1] = '\0';
            return;
        }
    }

    // hopefully we never get here, every log_0000.csv ... log_9999.csv exists
    printf("ERROR: No available log file slots!\n");
    snprintf(current_log_filename, sizeof(current_log_filename), "0:/data/log_overflow.csv");
}

const char* buildMetaFilename(const char* log_filename) {
    static char meta_fn[64];
    size_t n = strlen(log_filename);

    // Strip ".csv" and add "_channels.csv"
    snprintf(meta_fn, sizeof(meta_fn), "%.*s_channels.csv", (int)(n - 4), log_filename);

    return meta_fn;
}

FRESULT createChannelMetaFile(const char* log_filename) {
    const char *meta_fn = buildMetaFilename(log_filename);
    FRESULT fr;
    char line[128];

    snprintf(line, sizeof(line), "name,unit,min,max\n");
    fr = sd_append_file(meta_fn, line);
    if (fr != FR_OK) return fr;

    for (uint16_t i = 0; i < meta_count; ++i) {
        snprintf(line, sizeof(line), "%s,%s,%.6g,%.6g\n",
                 meta[i].name, meta[i].unit, meta[i].min_v, meta[i].max_v);
        fr = sd_append_file(meta_fn, line);
        if (fr != FR_OK) return fr;
    }

    return FR_OK;
}

void stopSDLogging();
void sdLogValue(const char* name, float value);

static void loggerTask(void *argument) {
    FRESULT fr;

    // mount card
    fr = sd_mount();
    if (fr != FR_OK) {
        vTaskDelete(NULL);
    }

    // ensure directory exists
    fr = sd_create_directory("data");
    if (fr != FR_OK && fr != FR_EXIST) {
        sd_unmount();
        vTaskDelete(NULL);
    }

    pickNextLogFile();
    fr = createChannelMetaFile(current_log_filename);
    if (fr != FR_OK) {
        printf("ERROR: Failed to create channel meta file (%d)\n", fr);
    }

    // mark logger active
    logger_ready = 1;

    for (;;)
	{
		while (log_tail != log_head)
		{
			const char *line = log_queue[log_tail].line;

			fr = sd_append_file(current_log_filename, line);
			if (fr != FR_OK) {
				logger_ready = 0;
				stopSDLogging();
				vTaskDelete(NULL);
			}

			log_tail = (log_tail + 1) % LOG_QUEUE_LEN;
		}

		vTaskDelay(pdMS_TO_TICKS(LOGGER_TASK_DELAY_MS));
	}

    stopSDLogging();
}

void initSdLogging() {
    xTaskCreate(
        loggerTask,
        "LoggerTask",
        1028,
        NULL,
        tskIDLE_PRIORITY, // lowest priority
        NULL
    );
}

/**
 * @brief Gracefully stop SD logging and unmount the card.
 * @warning Call only when producers have stopped; may block and cause faults if used concurrently.
 */
void stopSDLogging() {
    logger_ready = 0;
    f_sync(&log_file);
    f_close(&log_file);
    sd_unmount();
}

void sdLogValue(const char* name, float value) {
    if (!logger_ready) {
        return; // logger task not mounted / file not open yet
    }

    uint16_t next = (log_head + 1) % LOG_QUEUE_LEN;
    if (next == log_tail) {
        // buffer full -> drop newest
        log_drops++;
        return;
    }

    char ts[32];
    int ts_len = rtc_iso_ts(ts, sizeof(ts));
    if (ts_len <= 0) {
        return;
    }

    int n = snprintf(log_queue[log_head].line,
                     LOG_LINE_MAX,
                     "%s,%s,%.4f\n",
                     ts, name, value);

    if (n <= 0 || n >= LOG_LINE_MAX) {
        return;
    }

    log_head = next;
}

