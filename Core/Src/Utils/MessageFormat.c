#include "../../Inc/Utils/MessageFormat.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../Inc/stm32f7xx_hal.h"

// Telemetry output method selection
#define TELEMETRY_DISABLED  0
#define TELEMETRY_UART      1
#define TELEMETRY_USB_CDC   2

// Change this to switch telemetry output method
#define TELEMETRY_METHOD TELEMETRY_UART

#if TELEMETRY_METHOD == TELEMETRY_USB_CDC
// Include USB CDC when available
#include "../../../USB_DEVICE/App/usbd_cdc_if.h"
#endif

#define MESSAGE_FORMAT_BUFFER_SIZE 256
#define MESSAGE_FORMAT_QUEUE_DEPTH 64

typedef struct {
    uint16_t length;
    uint8_t data[MESSAGE_FORMAT_BUFFER_SIZE];
} MessagePacket;

static MessagePacket message_queue[MESSAGE_FORMAT_QUEUE_DEPTH];
static volatile uint8_t message_queue_head = 0;
static volatile uint8_t message_queue_tail = 0;
static volatile bool message_queue_active = false;
static bool message_queue_initialized = false;

extern UART_HandleTypeDef huart3;

static const char* getTypeString(MessageType type) {
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
    return type_strings[type];
}

static inline uint8_t messageQueueNextIndex(uint8_t index) {
    return (uint8_t)((index + 1U) % MESSAGE_FORMAT_QUEUE_DEPTH);
}

static inline bool messageQueueEmpty(void) {
    return message_queue_head == message_queue_tail;
}

static inline bool messageQueueFull(void) {
    return messageQueueNextIndex(message_queue_tail) == message_queue_head;
}

static void initializeMessageQueue(void) {
    if (!message_queue_initialized) {
        message_queue_head = 0;
        message_queue_tail = 0;
        message_queue_active = false;
        message_queue_initialized = true;
    }
}

static void startNextQueuedMessage(void);

static void completeQueuedMessage(void) {
    message_queue_head = messageQueueNextIndex(message_queue_head);
    message_queue_active = false;
}

static int enqueueMessage(const char* buffer, int len) {
    if (buffer == NULL || len <= 0) {
        return 0;
    }

    initializeMessageQueue();

    if (huart3.Instance == NULL || huart3.gState == HAL_UART_STATE_RESET) {
        // UART is not initialized yet; drop early telemetry and avoid blocking.
        return len;
    }

    if (len > MESSAGE_FORMAT_BUFFER_SIZE) {
        len = MESSAGE_FORMAT_BUFFER_SIZE;
    }

    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    if (messageQueueFull()) {
        message_queue_head = messageQueueNextIndex(message_queue_head);
    }

    MessagePacket* packet = &message_queue[message_queue_tail];
    memcpy(packet->data, buffer, (size_t)len);
    packet->length = (uint16_t)len;
    message_queue_tail = messageQueueNextIndex(message_queue_tail);
    bool should_start = !message_queue_active;

    if (!primask) {
        __enable_irq();
    }

    if (should_start) {
        startNextQueuedMessage();
    }

    return len;
}

static void startNextQueuedMessage(void) {
    if (message_queue_active || messageQueueEmpty()) {
        return;
    }

    MessagePacket* packet = &message_queue[message_queue_head];
    if (huart3.gState != HAL_UART_STATE_READY) {
        return;
    }

    if (HAL_UART_Transmit_DMA(&huart3, packet->data, packet->length) == HAL_OK) {
        message_queue_active = true;
    }
}

static inline bool messageQueueIdle(void) {
    return !message_queue_active && messageQueueEmpty();
}

static bool waitForTransmitReady(uint32_t timeout_ms) {
    uint32_t start = HAL_GetTick();
    while (1) {
        if (huart3.gState == HAL_UART_STATE_READY && messageQueueIdle()) {
            return true;
        }
        if ((HAL_GetTick() - start) >= timeout_ms) {
            return false;
        }
        HAL_Delay(1);
    }
}

int sendMessageRaw(const char* buffer, int len) {
#if TELEMETRY_METHOD == TELEMETRY_DISABLED
    (void)buffer;
    (void)len;
    return 0;
#else
    return enqueueMessage(buffer, len);
#endif
}

int sendMessageRawSync(const char* buffer, int len, uint32_t timeout_ms) {
#if TELEMETRY_METHOD == TELEMETRY_DISABLED
    (void)buffer;
    (void)len;
    (void)timeout_ms;
    return 0;
#elif TELEMETRY_METHOD == TELEMETRY_UART
    if (buffer == NULL || len <= 0) {
        return 0;
    }

    if (len > MESSAGE_FORMAT_BUFFER_SIZE) {
        len = MESSAGE_FORMAT_BUFFER_SIZE;
    }

    if (!waitForTransmitReady(timeout_ms)) {
        return 0;
    }

    if (HAL_UART_Transmit(&huart3, (uint8_t*)buffer, (uint16_t)len, timeout_ms) == HAL_OK) {
        return len;
    }

    return 0;
#elif TELEMETRY_METHOD == TELEMETRY_USB_CDC
    if (buffer == NULL || len <= 0) {
        return 0;
    }
    CDC_Transmit_FS((uint8_t*)buffer, (uint16_t)len);
    return len;
#endif
}

int sendMessageSync(const char* sender, MessageType type, const char* format, ...) {
#if TELEMETRY_METHOD == TELEMETRY_DISABLED
    (void)sender;
    (void)type;
    (void)format;
    return 0;
#else
    char buffer[MESSAGE_FORMAT_BUFFER_SIZE];
    int offset = snprintf(buffer, MESSAGE_FORMAT_BUFFER_SIZE - 3,
                          "Sender:%s;InfoType:%s;Content:", sender, getTypeString(type));
    if (offset < 0) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    vsnprintf(buffer + offset, MESSAGE_FORMAT_BUFFER_SIZE - offset - 3, format, args);
    va_end(args);

    size_t current_len = strnlen(buffer, MESSAGE_FORMAT_BUFFER_SIZE - 2);
    if (current_len + 2 < MESSAGE_FORMAT_BUFFER_SIZE) {
        buffer[current_len++] = '\r';
        buffer[current_len++] = '\n';
        buffer[current_len] = '\0';
    } else if (current_len < MESSAGE_FORMAT_BUFFER_SIZE) {
        buffer[current_len++] = '\n';
        buffer[current_len] = '\0';
    }

    return sendMessageRawSync(buffer, (int)current_len, 2000);
#endif
}

void sendMessage(const char* sender, MessageType type, const char* format, ...) {
#if TELEMETRY_METHOD == TELEMETRY_DISABLED
    // Telemetry disabled for maximum performance
    return;
    
#elif TELEMETRY_METHOD == TELEMETRY_UART
    char buffer[MESSAGE_FORMAT_BUFFER_SIZE];
    int offset = snprintf(buffer, MESSAGE_FORMAT_BUFFER_SIZE - 3,
                          "Sender:%s;InfoType:%s;Content:", sender, getTypeString(type));
    if (offset < 0) {
        return;
    }

    va_list args;
    va_start(args, format);
    vsnprintf(buffer + offset, MESSAGE_FORMAT_BUFFER_SIZE - offset - 3, format, args);
    va_end(args);

    size_t current_len = strnlen(buffer, MESSAGE_FORMAT_BUFFER_SIZE - 2);
    if (current_len + 2 < MESSAGE_FORMAT_BUFFER_SIZE) {
        buffer[current_len++] = '\r';
        buffer[current_len++] = '\n';
        buffer[current_len] = '\0';
    } else if (current_len < MESSAGE_FORMAT_BUFFER_SIZE) {
        buffer[current_len++] = '\n';
        buffer[current_len] = '\0';
    }

    sendMessageRaw(buffer, (int)current_len);

#elif TELEMETRY_METHOD == TELEMETRY_USB_CDC
    // USB CDC method - 100x faster than UART
    char buffer[256];
    
    // Format complete message in buffer
    int offset = snprintf(buffer, 100, "Sender:%s;InfoType:%s;Content:", 
                         sender, getTypeString(type));
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer + offset, 150, format, args);
    va_end(args);
    
    strcat(buffer, "\r\n");
    
    // Send via USB CDC - 100x faster than UART!
    CDC_Transmit_FS((uint8_t*)buffer, strlen(buffer));
    
#endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        completeQueuedMessage();
        bool has_more = !messageQueueEmpty();
        if (!primask) {
            __enable_irq();
        }
        if (has_more) {
            startNextQueuedMessage();
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        message_queue_active = false;
        bool has_more = !messageQueueEmpty();
        if (!primask) {
            __enable_irq();
        }
        if (has_more) {
            startNextQueuedMessage();
        }
    }
}
