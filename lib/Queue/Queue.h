#ifndef Queue_h
#define Queue_h

#include "Arduino.h"

enum sensorType{soilS, tempS, humidS, lightS};

extern uint8_t sreading[2];

// define Q
extern xQueueHandle controlQ, soilQ, tempQ, humidQ, lightQ;

// create Q method
void createQ();

#endif