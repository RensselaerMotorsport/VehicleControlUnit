#ifndef RENSSELAERMOTORSPORT_CANSENSOR_H
#define RENSSELAERMOTORSPORT_CANSENSOR_H

#include "Sensor.h"

typedef struct {
    int bus_number;
    int baud_rate;
    Message* messages;
} Bus;

typedef struct {
    Sensor base;
    Bus bus;
    char node[64];
} CANSensor;

void initBus(Bus* bus, int bus_number, int baud_rate, DBC* dbc);

int enableBus(Bus* bus);

int disableBus(Bus* bus);

void initCANSensor(CANSensor* sensor, const char* name, const char* node, int hz, int bus);

int enableCANSensor(CANSensor* sensor);

int disableCANSensor(CANSensor* sensor);

#endif
