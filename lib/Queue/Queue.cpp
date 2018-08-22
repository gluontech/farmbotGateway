#include "queue.h"

xQueueHandle controlQ, soilQ, tempQ, humidQ, lightQ;
uint8_t sreading[2];

void createQ(){
  
  controlQ = xQueueCreate(10, sizeof(sreading));
  soilQ = xQueueCreate(5, sizeof(sreading));
  tempQ = xQueueCreate(5, sizeof(sreading));
  humidQ = xQueueCreate(5, sizeof(sreading));
  lightQ = xQueueCreate(5, sizeof(sreading));

  
 if(controlQ == NULL) { Serial.println("Error creating Control queue");}
  if(soilQ == NULL) { Serial.println("Error creating Soil queue");}
  if(tempQ == NULL) { Serial.println("Error creating Temp queue");}
  if(humidQ == NULL) { Serial.println("Error creating Humid queue");}
  if(lightQ == NULL) { Serial.println("Error creating Light queue");}
 }
