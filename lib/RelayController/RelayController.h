#ifndef RelayController_h
#define RelayController_h

#include "Arduino.h"
#include "Timer.h"
extern Timer t;

void setupPinMode(void);

void turnOffWater();
void turnOffWater2();
void turnOffHeat();
void turnOffHeat2();
void turnOffFan(); 
void turnOffHumidifier();

void waterControl();
void waterControl2();
void heatControl();
void heatControl2();
void fanControl();
void humidControl();

#endif