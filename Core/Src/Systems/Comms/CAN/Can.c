#ifndef TEST_MODE
#include "stm32f7xx_it.h"
#endif

#include "../../../../Inc/Systems/Comms/Can/Can.h"
#include "../../../../Inc/Systems/Comms/Can/DBCParser.h"
#include "../../../../Inc/Utils/Common.h"
#include "../../../../Inc/Utils/MessageFormat.h"
#include "../../../../Inc/Outputs/DigitalOutput.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef TEST_MODE
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#endif

// CAN message list
CAN_MessageList can_messages[MAX_BUS];
// Temporary test digital output light
static DigitalOutput temp_light_storage;
static DigitalOutput* temp_light = &temp_light_storage;

// Pool + prioritized queues
#define CAN_POOL_SIZE 32
#define CAN_MESSAGE_BUFFER_SIZE (sizeof(CAN_Message) + sizeof(CAN_Signal) * MAX_SIGNALS)

typedef struct {
    CAN_Message* message;
    int id;
    CANBus bus;
    uint8_t from_pool; // 1 if buffer came from pool, 0 if malloc'd
} CAN_Message_QueueItem;


static uint8_t can_message_pool_mem[CAN_POOL_SIZE][CAN_MESSAGE_BUFFER_SIZE];
static QueueHandle_t canPoolQueue = NULL;
static QueueHandle_t canMessageQueue = NULL;
static TaskHandle_t canProcessingTaskHandle = NULL;

int init_CANBus(CANBus bus) {
    // Initialize the CAN message list
    can_messages[bus].bus = bus;
    can_messages[bus].num_messages = 0;

    initDigitalOutput(temp_light, "Test light", 10, 1);

    // Initialize pool and priority queues
#ifndef TEST_MODE
    if (canPoolQueue == NULL) {
        canPoolQueue = xQueueCreate(CAN_POOL_SIZE, sizeof(void*));
        if (canPoolQueue == NULL) {
            printf("Failed to create CAN pool queue\r\n");
        } else {
            // Push all pool pointers into the free pool queue
            for (int i = 0; i < CAN_POOL_SIZE; i++) {
                void* ptr = (void*)can_message_pool_mem[i];
                xQueueSend(canPoolQueue, &ptr, 0);
            }
        }
    }

    if (canMessageQueue == NULL) {
        canMessageQueue = xQueueCreate(128, sizeof(CAN_Message_QueueItem));
        if (canMessageQueue == NULL) {
            printf("Failed to create CAN message queue\r\n");
        }
    }

    if (canProcessingTaskHandle == NULL) {
        BaseType_t res = xTaskCreate(CAN_MessageProcessingTask, "CANProc", 128, NULL, tskIDLE_PRIORITY + 2, &canProcessingTaskHandle);
        if (res != pdPASS) {
            printf("Failed to create CAN processing task\r\n");
        }
    }
#endif

    return 0;
}

void call_func_from_message(CAN_Message* can_message, int id){
    // Enqueue the message for processing by the CAN processing task.
    if (canMessageQueue == NULL || canPoolQueue == NULL) {
        // If queue is not initialized, fallback to direct processing to avoid dropping messages
        printf("CAN queue not initialized, processing directly\r\n");
        switch(id){
            case(123):
                printf("Func call goes here\r\n");
                if(can_message -> data[0] == 1){
                    writeDigitalOutputData(temp_light, 1);
                }
                else if(can_message -> data[0] == 0){
                    writeDigitalOutputData(temp_light, 0);
                }
                else{
                    printf("Something went wrong...\r\n");
                }
                break;
            case(124):
                printf("Second func call\r\n");
                break;
        }
        free(can_message);
        return;
    }

    CAN_Message_QueueItem item;
    item.message = can_message;
    item.id = id;
    item.bus = CAN_1;
    item.from_pool = 0; // caller-provided buffer

    BaseType_t sent = xQueueSendToBack(canMessageQueue, &item, 0);
}

int load_dbc_file(CANBus bus, const unsigned char* filename)
{
    // Load the DBC file
    #ifdef TEST_MODE
    // unsigned char* dbc_contents = fopen(filename, "w");
    #else
    unsigned char* dbc_contents = filename;
    #endif
//    //printf("Loading DBC file: %c\r\r\n", filename[0]);
    
    #ifndef TEST_MODE
    // Parse the DBC file
    parseDbcFile(&can_messages[bus], dbc_contents);
    #endif

	#ifdef TEST_MODE
    // fclose(dbc_contents);
	#endif

    print_CAN_MessageList(&can_messages[bus]);
    return 0;
}

int add_message(CANBus bus, CAN_Message_Template message) {
    // Add the message to the list
    can_messages[bus].messages[can_messages[bus].num_messages] = message;
    can_messages[bus].num_messages++;
    return 0;
}

int add_message_lop(CANBus bus, int id, int dlc, int ide, int rtr, const char* name, const char* sender, int signal_count, CAN_Signal_Template* signals)
{
    // Create a CAN message template
    CAN_Message_Template message;
    message.id = id;
    message.dlc = dlc;
    message.ide = ide;
    message.rtr = rtr;
    strcpy(message.name, name);
    strcpy(message.sender, sender);
    message.signal_count = signal_count;
    message.payload_length = 0;

    // Copy the signals
    for (int i = 0; i < signal_count; i++) {
        message.signals[i] = signals[i];
        message.payload_length += signals[i].length;
    }

    // Add the message to the list
    return add_message(bus, message);
}

// Send a CAN message
int send_CAN_message(CANBus bus, CANProtocol protocol, uint32_t id, uint8_t* data, uint8_t len) {
    CAN_TxHeaderTypeDef TxHeader;
    if (protocol == CAN_2A) {
        TxHeader.StdId = id;
        TxHeader.IDE = CAN_ID_STD;
    } else if (protocol == CAN_2B) {
        TxHeader.ExtId = id;
        TxHeader.IDE = CAN_ID_EXT;
    } else {
        //printf("Invalid CAN protocol\r\n");
        return -1;
    }

    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = len;

    // Print the message
    //printf(ANSI_COLOR_YELLOW "Sending CAN Message" ANSI_COLOR_RESET ": ID: %d, DLC: %d, Data: ", id, len);
    for (int i = 0; i < len; i++) {
        //printf("%02X ", data[i]);
    }
    //printf("\r\n");

    #ifndef TEST_MODE
    // Send the CAN message
    if (send_CAN_message_helper(bus, &TxHeader, data) != 0) {
        return -2;
    }
    #endif
    return 0;
}

// Receive a CAN message from the Callback
void receive_CAN_message(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData, CANBus bus) {
    // Send telemetry message for received CAN data
    char data_hex[17]; // 8 bytes * 2 hex chars + null terminator
    for (int i = 0; i < RxHeader->DLC && i < 8; i++)
    {
        sprintf(&data_hex[i * 2], "%02X", RxData[i]);
    }
    data_hex[RxHeader->DLC * 2] = '\0';

    // Send properly formatted CAN RX message
    sendMessage("CAN", MSG_CAN_RX, "ID:0x%lX;DLC:%lu;Data:%s",
                RxHeader->StdId, RxHeader->DLC, data_hex);

    
    void* bufptr = NULL;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (canPoolQueue != NULL && xQueueReceiveFromISR(canPoolQueue, &bufptr, &xHigherPriorityTaskWoken) == pdTRUE) {
        CAN_Message* can_message = (CAN_Message*)bufptr;
        can_message->header = *RxHeader;
        memcpy(can_message->data, RxData, 8);

        CAN_Message_QueueItem item;
        item.message = can_message;
        item.id = RxHeader->StdId;
        item.bus = bus;
        item.from_pool = 1;

        BaseType_t sent = pdFAIL;
            sent = xQueueSendToBackFromISR(canMessageQueue, &item, &xHigherPriorityTaskWoken);
        if (sent == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return;
        } else {
            // Failed to enqueue - return buffer to pool and drop
            xQueueSendToBackFromISR(canPoolQueue, &bufptr, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            printf("CAN RX: queue full, dropped message\r\n");
            return;
        }
    } else {
        printf("CAN RX: pool empty, dropping message\r\n");
        return;
    }
}

static void CAN_MessageProcessingTask(void* params) {
    (void)params;
    CAN_Message_QueueItem item;
    for(;;) {
#ifndef TEST_MODE
        if (canMessageQueue != NULL && xQueueReceive(canMessageQueue, &item, portMAX_DELAY) == pdTRUE) {
            // got message
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        CAN_Message* can_message = item.message;
        int id = item.id;

        // Handle message
        parseMessage(&can_messages[item.bus], can_message);

        //Perform action based on CAN id
        switch(id){
            case 123:
                if (can_message != NULL) {
                    if(can_message->data[0] == 1){
                        writeDigitalOutputData(temp_light, 1);
                    } else if(can_message->data[0] == 0){
                        writeDigitalOutputData(temp_light, 0);
                    }
                }
                break;
            case 124:
                printf("Second test func call for ID 124\r\n");
                break;
            default:
                break;
        }

        // Return buffer to pool or free
        if (item.from_pool && canPoolQueue != NULL) {
            void* ptr = (void*)can_message;
            xQueueSend(canPoolQueue, &ptr, 0);
        } else {
            if (can_message != NULL) free(can_message);
        }
#else
        // No RTOS
        vTaskDelay(pdMS_TO_TICKS(100));
#endif
    }
}

// Parse a CAN message
void parseMessage(CAN_MessageList* messages, CAN_Message* can_message) {
    // Find the message in the list
    // TODO: Implement a hash table or map for faster lookup
    for (int i = 0; i < messages->num_messages; i++) {
        CAN_Message_Template* msg = &messages->messages[i];
        if (msg->id == can_message->header.StdId || msg->id == can_message->header.ExtId) {
            // Parse the signals of the message
            can_message->template = msg;
            parseSignals(msg, can_message);
            return;
        }
    }
}

// Parse the signals of a CAN message
void parseSignals(CAN_Message_Template* message, CAN_Message* can_message) {
    // Find the signals in the message
    for (int i = 0; i < message->signal_count; i++) {
        CAN_Signal_Template* signal = &message->signals[i];
        // Parse the signal
        parseSignal(signal, &can_message->signals[i], can_message);
    }
}

// Parse a CAN signal
void parseSignal(CAN_Signal_Template* signal, CAN_Signal* can_signal, CAN_Message* can_message) {
    can_signal->template = signal;

    // Extract the raw data from the message
    uint64_t raw_data = 0;
    for (int i = signal->start_bit; i < signal->start_bit + signal->length; i++) {
        raw_data |= ((can_message->data[i / 8] >> (i % 8)) & 1) << (i - signal->start_bit);
    }

    // Check if the signal is big endian
    if (signal->endian == 1) {
        // Reverse the bits
        uint64_t reversed_data = 0;
        for (int i = 0; i < signal->length; i++) {
            reversed_data |= ((raw_data >> i) & 1) << (signal->length - i - 1);
        }
        raw_data = reversed_data;
    }

    // Check if the signal is signed
    if (signal->isSigned) {
        // Sign extend the data
        raw_data = (raw_data << (64 - signal->length)) >> (64 - signal->length);
    }

    // Scale and offset the data
    can_signal->value = (raw_data * signal->scale) + signal->offset;

}

void print_CAN_Messages_Lists() {
    for (int i = 0; i < MAX_BUS; i++) {
        //printf("Printing CAN Message List for bus %d with %d messages\r\n", i, can_messages[i].num_messages);
        for (int j = 0; j < can_messages[i].num_messages; j++) {
            const CAN_Message_Template* msg = &can_messages[i].messages[j];
            //printf(ANSI_COLOR_GREEN "Message" ANSI_COLOR_RESET ": %s (ID: %d, DLC: %d, Sender: %s, SIGs: %d)\r\n", msg->name, msg->id, msg->dlc, msg->sender, msg->signal_count);
            for (int k = 0; k < msg->signal_count; k++) {
                const CAN_Signal_Template* sig = &msg->signals[k];
                //printf("\t" ANSI_COLOR_BLUE "Signal" ANSI_COLOR_RESET ": %s (Start bit: %d, Length: %d, Endain: %d, Signed: %c,\r\n\t\tScale: %f, Offset: %f, Min: %f, Max: %f, \r\n\t\tUnit: %s, Reciever: %s)\r\n", sig->name, sig->start_bit, sig->length, sig->endian, sig->isSigned, sig->scale, sig->offset, sig->min, sig->max, sig->unit, sig->reciever);
            }
            //printf("\r\n");
        }
        if (can_messages[i].num_messages == 0) {
            //printf("No messages on bus %d\r\n", i);
        }
    }
}
