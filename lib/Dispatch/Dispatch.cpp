#include "Dispatch.h"


// receive data from queue and dispatch controllers
void taskDispatch( void * parameter)
{
  //controlID sdata;
  uint8_t sreading[2];
  BaseType_t xStatus; /* status of receiving data */
  
  while(1)
  {
    xStatus = xQueueReceive(controlQ, &sreading, pdMS_TO_TICKS(80)); 
     if(xStatus == pdPASS)
    {
       switch(sreading[0])
        {
          case soilS:
            if (sreading[1] <= 45) waterControl();
            else if (sreading[1] > 60) turnOffWater();
            break;
          case tempS:
            if (sreading[1] <= 15) heatControl();
            else if (sreading[1] > 32) fanControl();
            break;
          case humidS:
            if (sreading[1] <= 25) humidControl();
            else if (sreading[1] > 75) fanControl();
            break;
          case lightS:
            // calculate average full sun hours
            break;
        }
    }
     t.update();
  }
    vTaskDelete( NULL );  
}