#ifndef RelayController_h
#define RelayController_h

#include "Arduino.h"
#include "Timer.h"
extern Timer t;

void setupPinMode(void);
 void turnOffWater();
void turnOffHeat();
void turnOffFan(); 
void waterControl();
void heatControl();
void fanControl();
void humidControl();

#endif