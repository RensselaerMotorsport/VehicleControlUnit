#include "../../../../Inc/Systems/External/Can/Can.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CAN handle
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
CAN_HandleTypeDef hcan3;

// Receive message buffers
CAN_RxHeaderTypeDef RxHeader1;
CAN_RxHeaderTypeDef RxHeader2;
CAN_RxHeaderTypeDef RxHeader3;
uint8_t RxData1[8];
uint8_t RxData2[8];
uint8_t RxData3[8];

int init_CANBus(CANBus bus, CANProtocol protocol, CANBaudrate baudrate)
{
    CAN_HandleTypeDef hcan; 
    if (bus == CAN_1) {
	    __HAL_RCC_CAN1_CLK_ENABLE();  // Enable clock for CAN1
        hcan.Instance = CAN1;
    } else if (bus == CAN_2) {
	    __HAL_RCC_CAN2_CLK_ENABLE();  // Enable clock for CAN1
        hcan.Instance = CAN2;
    } else if (bus == CAN_3) {
	    __HAL_RCC_CAN3_CLK_ENABLE();  // Enable clock for CAN1
        hcan.Instance = CAN3;
    } else {
        return -1;
    }

    if (baudrate == CAN_100KBPS) {
        hcan.Init.Prescaler = 5;
        hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
        hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
        hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    } else if (baudrate == CAN_125KBPS) {
        hcan.Init.Prescaler = 4;
        hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
        hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
        hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    } else if (baudrate == CAN_250KBPS) {
        hcan.Init.Prescaler = 2;
        hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
        hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
        hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    } else if (baudrate == CAN_500KBPS) {
        hcan.Init.Prescaler = 3;
        hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
        hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
        hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    } else if (baudrate == CAN_1MBPS) {
        hcan.Init.Prescaler = 1;
        hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
        hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
        hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    } else {
        return -1;
    }

    hcan.Init.Mode = CAN_MODE_NORMAL; // Normal mode
    hcan.Init.AutoBusOff = ENABLE; // Enable auto bus off
    hcan.Init.AutoRetransmission = ENABLE; // Enable auto retransmission
    hcan.Init.ReceiveFifoLocked = DISABLE; // Disable FIFO locked
    hcan.Init.TransmitFifoPriority = DISABLE; // Disable FIFO priority
    hcan.Init.TimeTriggeredMode = DISABLE; // Disable time triggered mode
    hcan.Init.AutoWakeUp = DISABLE; // Disable auto wake up

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        return -1;
    }

    // Configure the CAN filter
    CAN_FilterTypeDef filterConfig;
    filterConfig.FilterBank = 0;                 // Filter bank
    filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    filterConfig.FilterIdHigh = 0x0000;
    filterConfig.FilterIdLow = 0x0000; // Accept all IDs
    filterConfig.FilterMaskIdHigh = 0x0000;
    filterConfig.FilterMaskIdLow = 0x0000; // Accept all bits
    filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    filterConfig.FilterActivation = ENABLE;
    filterConfig.SlaveStartFilterBank = 14;

    // Configure Filter
    if (HAL_CAN_ConfigFilter(&hcan, &filterConfig) != HAL_OK) {
    	printf("0\r\n");
    }

    // Stop existing CAN
    HAL_CAN_Stop(&hcan);

    // Start CAN
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        printf("CAN Start Error: ErrorCode = 0x%lX\r\n", hcan.ErrorCode);
    }

    if (bus == CAN_1) {
        // Enable CAN IRQ
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    } else if (bus == CAN_2) {
        // Enable CAN IRQ
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    } else if (bus == CAN_3) {
        // Enable CAN IRQ
        HAL_NVIC_EnableIRQ(CAN3_RX0_IRQn);
    }

    // Start IRQ for CAN Rx
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
    	printf("interrupt init failed\r\n");
    }

    if (bus == CAN_1) {
        hcan1 = hcan;
    } else if (bus == CAN_2) {
        hcan2 = hcan;
    } else if (bus == CAN_3) {
        hcan3 = hcan;
    }

    // Initialize the CAN message list
    can_messages[bus].bus = bus;
    can_messages[bus].num_messages = 0;
    
    return 0;
}

/**
* @brief Loads a DBC file into the CAN database. Makes the messages and signals and stores them in the list
* 
* @param bus The CAN bus to load the DBC file into
* @param filename The name of the DBC file to load
* @return int 0 if the DBC file was loaded successfully, -1 if the file could not be opened
*/
int load_dbc_file(CANBus bus, const char* filename)
{
    // Load the DBC file
    FILE* dbc_file = fopen(filename, "r");
    if (dbc_file == NULL) {
        return -1;
    }
    // Parse the DBC file
    parseDbcFile(&can_messages[bus], filename);
}

int add_message(CANBus bus, int id, int dlc, int ide, int rtr, const char* name, const char* sender)
{
    // Initialize the message
    initMessage(&can_messages[bus], id, dlc, ide, rtr, name, sender);

    // Add the message to the list
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (can_messages[bus][i].id == 0) {
            can_messages[bus][i] = can_messages[bus];
            return 0;
        }
    }
    return -1;
}

// Send a CAN message
int send_CAN_message(CANBus bus, CANProtocol protocol, uint32_t id, uint8_t* data, uint8_t len)
{
    CAN_HandleTypeDef hcan;
    if (bus == CAN_1) {
        hcan = hcan1;
    } else if (bus == CAN_2) {
        hcan = hcan2;
    } else if (bus == CAN_3) {
        hcan = hcan3;
    } else {
        printf("Invalid CAN bus\r\n");
        return -1;
    }

    CAN_TxHeaderTypeDef TxHeader;
    if (protocol == CAN_2A) {
        TxHeader.StdId = id;
        TxHeader.IDE = CAN_ID_STD;
    } else if (protocol == CAN_2B) {
        TxHeader.ExtId = id;
        TxHeader.IDE = CAN_ID_EXT;
    } else {
        printf("Invalid CAN protocol\r\n");
        return -1;
    }

    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = len;
    TxHeader.TransmitGlobalTime = DISABLE;

    uint32_t TxMailbox;
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, data, &TxMailbox) != HAL_OK) {
        printf("Failed to send CAN message\r\n");
        return -1;
    }
    return 0;
}

// Receive a CAN message from the Callback
void receive_CAN_message(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData, CANBus bus) {
    // Create a CAN message
    CAN_Message* can_message = malloc(sizeof(CAN_Message));
    can_message->header = *RxHeader;
    memcpy(can_message->data, RxData, 8);

    // Parse the message
    parseMessage(can_messages[bus], message);

    // Print out the contents of the message
    printf("Received CAN message on bus %d with ID %d and data: ", bus, can_message->header.StdId);
    for (int i = 0; i < can_message->header.DLC; i++) {
        printf("%d ", can_message->data[i]);
    }
    printf("\r\n");
    
    // Print out the signals
    for (int i = 0; i < can_message->template->signal_count; i++) {
        Signal* signal = &can_message->signals[i];
        printf("\t%s: %f \r\n", signal->template->name, signal->value);
        free(signal);
    }
    printf("\r\n");

    // Free the message
    free(can_message);
}

// Parse a CAN message
void parseMessage(CAN_MessageList* messages, CAN_Message* can_message) {
    // Find the message in the list
    // TODO: Implement a hash table or map for faster lookup
    for (int i = 0; i < messages->num_messages; i++) {
        CAN_Message_Template* msg = &messages->messages[i];
        if (msg->id == can_message->header.StdId || msg->id == can_message->header.ExtId) {
            // Parse the signals of the message
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
        parseSignal(signal, can_message->signals[i]);
    }
}

// Parse a CAN signal
void parseSignal(CAN_Signal_Template* signal, CAN_Signal* can_signal) {
    can_signal->template = signal;

    // Parse the signal data, including scaling and offset and endianess
    uint64_t raw_data = 0;
    for (int i = 0; i < signal->length; i++) {
        raw_data |= (can_signal->data[i] << (signal->start_bit + i));
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

// CAN1 IRQ Handler
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan1);
}

// CAN2 IRQ Handler
void CAN2_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan2);
}

// CAN3 IRQ Handler
void CAN3_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan3);
}

// Callback for receiving CAN messages
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader1, RxData1);
        receive_CAN_message(&RxHeader1, RxData1, CAN_1);
    } else if (hcan == &hcan2) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader2, RxData2);
        receive_CAN_message(&RxHeader2, RxData2, CAN_2);
    } else if (hcan == &hcan3) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader3, RxData3);
        receive_CAN_message(&RxHeader3, RxData3, CAN_3);
    }
}