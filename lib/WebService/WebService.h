#ifndef WebService_h
#define WebService_h

#include "Arduino.h"
#include "Queue.h"
#include "RelayController.h"

//void taskWebService(void * parameter);
void webServiceSetup();
void defaultThresholdSetup();

// automation controller
void taskAutoController(void * parameter);
#endif