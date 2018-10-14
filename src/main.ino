#include "EspNowSlave.h"
#include "WebService.h"

void setup() 
{
  Serial.begin(9600);

  setupPinMode();
  createQ();

   setupEspnow(); // setup SoftApp wifi hot spot
   defaultThresholdSetup();
   webServiceSetup();
   //bluetoothSetup();
  
// Task that receive data from queue and dispatch controler
  xTaskCreate(
    taskAutoController,
    "taskAutoController",
    2000,
    NULL,
    1,
    NULL);  
} 

void loop() {}

