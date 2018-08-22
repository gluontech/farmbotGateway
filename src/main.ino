#include "EspNowSlave.h"
#include "Dispatch.h"
#include "WebService.h"
 //#include "Bluetooth.h"


void setup() 
{
  Serial.begin(9600);

  setupPinMode();
  createQ();

   setupEspnow(); // setup SoftApp wifi hot spot
   webServiceSetup();
   //bluetoothSetup();
  
// Task that receive data from queue and dispatch controler
  xTaskCreate(
    taskDispatch,
    "taskDispatch",
    2000,
    NULL,
    1,
    NULL);  

  /* xTaskCreate(
    taskBluetooth,
    "taskBluetooth",
    3000,
    NULL,
    1,
    NULL); */
} 

void loop() {}

