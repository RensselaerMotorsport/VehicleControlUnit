#include "../../../Inc/Systems/External/BMS.h"
#include "../../../Inc/Systems/PrintHelpers.h"

#include <stdio.h>
#include "../../../Inc/Systems/Comms/Can/Can.h"

void initBms(Bms* bms, int hz, const char* dbcFn) {
   if (bms == NULL) return;

   initExternalSystem(&bms->extSystem, "Bms", hz, 0, NULL, NULL);

   bms->packVoltage = 0.0f;
   bms->packCurrent = 0.0f;
   bms->stateOfCharge = 0.0f;
   bms->cellVoltageMin = 0.0f;
   bms->cellVoltageMax = 0.0f;
   bms->cellTemperatureMin = 0.0f;
   bms->cellTemperatureMax = 0.0f;
   bms->totalPackCapacity = 0.0f;
   bms->remainingPackCapacity = 0.0f;
   bms->packHealth = 0.0f;
   bms->chargeStatus = IDLE;
   bms->extSystem.system.updateable.update = updateBms;
}

void assignBmsValue(Bms* bms, int id, float value, const char* name) {
   switch (id) {
       // TODO: Remember to update +1712
       case 0:
           if (strcmp(name, "Pack_Current") == 0) {
               bms->packCurrent = value;
           } else if (strcmp(name, "Pack_Inst_Voltage") == 0) {
               bms->packVoltage = value;
           } else if (strcmp(name, "Pack_SOC") == 0) {
               bms->stateOfCharge = value;
           } else if (strcmp(name, "Relay_State") == 0) {
               /*bms->relayState = (int)value;*/
           } else if (strcmp(name, "CRC_Checksum") == 0) {
               // TODO: Deal with this

               /*bms->crcChecksum = (int)value;*/
           }
           break;
       case 1:
           if (strcmp(name, "Pack_DCL") == 0) {
               /*bms->packDCL = value;*/
           } else if (strcmp(name, "Pack_CCL") == 0) {
               /*bms->packCCL = value;*/
           } else if (strcmp(name, "High_Temperature") == 0) {
               /*bms->highTemperature = value;*/
           } else if (strcmp(name, "Low_Temperature") == 0) {
               /*bms->lowTemperature = value;*/
           } else if (strcmp(name, "CRC_Checksum") == 0) {
               /*bms->crcChecksum = (int)value;*/
           }
           break;
       default:
           // Handle invalid signal case if needed
           break;
   }
}

int bmsTransferFunction(Bms* bms, CAN_Message* canData) {
//   int index = canData->messageId;
//   Message* message = &bms->dbc->messages[index-1712];
//
//   // Check if the message ID matches the expected one
//   if (message == NULL && message->id != canData->messageId) {
//       return 0;  // Message ID not found or doesn't match
//   }
//
//   // Decode signals within the message
//   for (int i = 0; i < message->signal_count; i++) {
//       Signal* signal = &message->signals[i];
//       float value = extractSignalValue(signal, canData->data);
//       assignBmsValue(bms, message->id, value, signal->name);
//   }
//   return 1;
	return 1;
}

// TODO: Implemented this
void updateBms(void* bms) {
//   Bms* myBms = (Bms*) bms;
   printf("BMS Update Not implemented\n");
   // CanMessage canData = fetchCanData(...);
   // parseCanData(&canData, canDataFn);
   //
   // // Call the transfer function to decode the message
   // if (!bmsTransferFunction(myBms, &canData)) {
   //     printf("Error: Transfer function failed.\n");
   // }
}

// @warning For testing and debugging purposes only
void updateBmsTest(void* bms, const char* canDataFn) {
//   Bms* myBms = (Bms*) bms;
//   CanMessage canData;
//   parseCanData(&canData, canDataFn);
//
//   // Call the transfer function to decode the message
//   if (!bmsTransferFunction(myBms, &canData)) {
//       printf("Update Test Error: Transfer function failed.\n");
//   }
}
