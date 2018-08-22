#include <esp_now.h>
#include <WiFi.h>
#include "EspnowSlave.h"
#include "Queue.h"


// Init ESP Now with fallback
void InitESPNow() 
{
  if (esp_now_init() == ESP_OK) 
   {
    Serial.println("ESPNow Init Success");
   }
  else 
  {
    Serial.println("ESPNow Init Failed");
    ESP.restart();   
  }
}

// config AP SSID
void configDeviceAP() 
{
  const char* ssid = "Gfarmbot";
  const char* pwd = "gluontech";
  int channel = 1;  
  bool result = WiFi.softAP(ssid, pwd, channel, 0);
  if (!result) 
  {
    Serial.println("wifi setup fails.");
  } 
  else 
  {
    Serial.println(ssid);   
    Serial.println(WiFi.softAPIP()); 
  }
}

// receiving data and send to Queue
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) 
{
  uint8_t sreading[2];
  BaseType_t controlQStatus, soilQStatus, tempQStatus, humidQStatus, lightQStatus;
  memcpy(&sreading, data, sizeof(sreading));
  
   controlQStatus = xQueueSend(controlQ, &sreading, pdMS_TO_TICKS(80) );
   if( controlQStatus != pdPASS ) Serial.print("data sent to control Q failed:");

  switch(sreading[0])
    {
      case soilS:
        Serial.print("Soil Moisture: "); Serial.println(sreading[1]);
        if (uxQueueSpacesAvailable(soilQ) > 0) {
          soilQStatus = xQueueSendToFront(soilQ, &sreading, pdMS_TO_TICKS(80) );
           if(soilQStatus != pdPASS ) Serial.print("data sent to Soil Q failed:");
          }
        else xQueueReset(soilQ);  // Q is full reset it */
        break;
      case tempS:
        Serial.print("Temperature: "); Serial.println(sreading[1]);
         if (uxQueueSpacesAvailable(tempQ) > 0)
         {
            tempQStatus = xQueueSendToFront(tempQ, &sreading, pdMS_TO_TICKS(80) );
            if( tempQStatus != pdPASS ) Serial.print("data sent to Temp Q failed:");
         }
         else xQueueReset(tempQ);  
        break;
      case humidS:
         Serial.print("Humidity: "); Serial.println(sreading[1]);
          if (uxQueueSpacesAvailable(humidQ) > 0)
          {
            humidQStatus = xQueueSendToFront(humidQ, &sreading, pdMS_TO_TICKS(80) );
            if( humidQStatus != pdPASS ) Serial.print("data sent to Humid Q failed:");
          }
           else xQueueReset(humidQ);  
         break;
      case lightS:
          Serial.print("Light: "); Serial.println(sreading[1]);
          if (uxQueueSpacesAvailable(lightQ) > 0)
          {
            lightQStatus = xQueueSendToFront(lightQ, &sreading, pdMS_TO_TICKS(80) );
            if(lightQStatus != pdPASS ) Serial.print("data sent to Light Q failed:");
          }
          else xQueueReset(lightQ); 
          break;
    }
}

// setup Espnow to receive data from master
void setupEspnow(void){
  WiFi.mode(WIFI_AP);  //Set device in AP mode
  // configure device AP mode
  configDeviceAP();
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);
   // This is the mac address of the Slave in AP Mode
  Serial.println("AP MAC: "); 
  Serial.print(WiFi.softAPmacAddress());
}