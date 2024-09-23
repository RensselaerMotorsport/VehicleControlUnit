#include "../../includes/CANSensor.h"
#include "../../includes/DBCParser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initBus(Bus* bus, int bus_number, int baud_rate, DBC* dbc) {
    bus->bus_number = bus_number;
    bus->baud_rate = baud_rate;
    bus->messages = dbc->messages;
}

int enableBus(Bus* bus) {
    printf("Bus %d enabled at %d baud\n", bus->bus_number, bus->baud_rate);
    return 1;
}

int disableBus(Bus* bus) {
    printf("Bus %d disabled\n", bus->bus_number);
    return 1;
}

void addMessageToBus(Bus* bus, Message* message) {
    bus->messages[bus->message_count] = message;
    bus->message_count++;
}

void initCANSensor(CANSensor* sensor, const char* name, const char* node, int hz, int bus) {
    initSensor(&sensor->base, name, hz);
    sensor-> bus = bus;
}

//how to implement with scheduler? we want to update the bus and then move over stuff to the sensor
// maybe scheduler needs actuator and bus input too?

int enableCANSensor(CANSensor* sensor) {
    // implement after ticket RMSW-46
    return 1;
}

int disableCANSensor(CANSensor* sensor) {
    // implement after ticket RMSW-46
    return 1;
}
