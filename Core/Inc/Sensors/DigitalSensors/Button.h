#ifndef __RENNSSELAERMOTORSPORT_BUTTON_H
#define __RENNSSELAERMOTORSPORT_BUTTON_H

#include "../DigitalSensor.h"

typedef struct {
	DigitalSensor base;
} Button;

void initButton(Button* button, const char* name, int hz, int port);
int getButtonReading(Button* button);

#endif // __RENNSSELAERMOTORSPORT_BUTTON_H
