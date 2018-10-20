
#include "FS.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "WebService.h"
#include "ArduinoJson.h"
#include <iostream>
#include <string>
#include <Preferences.h>
using namespace std; 


AsyncWebServer server(80);
Preferences preferences;
// default values
int16_t soilLow, soilHigh, tempLow, tempHigh, humidLow, humidHigh;
const char * strSoillow = "soillow";
const char * strSoilhigh = "soilhigh";
const char * strSoillow2 = "soillow2";
const char * strSoilhigh2 = "soilhigh2";
const char * strTemplow = "templow";
const char * strTemphigh = "temphigh";
const char * strHumidlow = "humidlow";
const char * strHumidhigh = "humidhigh";

String getSoil()
{
  uint8_t sreading[2];
   String returnStr;
   BaseType_t xStatus; 
   xStatus = xQueueReceive(soilQ, &sreading, pdMS_TO_TICKS(80)); 
   if(xStatus == pdPASS)   
   {
      returnStr =  (String)sreading[1];   
   }
   else
      returnStr = "0";

  return returnStr;
}

String getTemp()
{
  uint8_t sreading[2];
   String returnStr;
   BaseType_t xStatus; 
   xStatus = xQueueReceive(tempQ, &sreading, pdMS_TO_TICKS(80)); 
   if(xStatus == pdPASS)   
   {
      returnStr =  (String)sreading[1];
    
   }
   else
      returnStr = "0";

  return returnStr;
}

String getHumid()
{
  uint8_t sreading[2];
   String returnStr;
   BaseType_t xStatus; 
   xStatus = xQueueReceive(humidQ, &sreading, pdMS_TO_TICKS(80)); 
   if(xStatus == pdPASS)   
   {
      returnStr =  (String)sreading[1];
    
   }
   else
      returnStr = "0";

  return returnStr;
}

String getLight()
{
  uint8_t sreading[2];
   String returnStr;
   BaseType_t xStatus; 
   xStatus = xQueueReceive(lightQ, &sreading, pdMS_TO_TICKS(80)); 
   if(xStatus == pdPASS)   
   {
      returnStr =  (String)sreading[1];
    
   }
   else
      returnStr = "0";

  return returnStr;
}

String createJsonResponse()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["soil"] = getSoil();
  root["temperature"] = getTemp();
  root["humidity"] = getHumid();
  root["light"] =  getLight();
   
  String json;
  root.printTo(json);
  return json;
}

// store default threshold to flash mem 
void defaultThresholdSetup()
{
  preferences.begin("auto_threshold",false);
  preferences.putInt(strSoillow, 45);
  preferences.putInt(strSoilhigh, 60);
  preferences.putInt(strSoillow2, 45);
  preferences.putInt(strSoilhigh2, 60);
  preferences.putInt(strTemplow, 15);
  preferences.putInt(strTemphigh, 32);
  preferences.putInt(strHumidlow, 20);
  preferences.putInt(strHumidhigh, 80);
  preferences.end();
}


void webServiceSetup()
{
  server.on("/getdata", HTTP_GET, [](AsyncWebServerRequest *request)
        {
           String jsonData = createJsonResponse();
           request->send(200, "text/json",  jsonData);
       });

 
  // water relay on
   server.on("/put/wateron", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           waterControl();
        });

 // water relay off
  server.on("/put/wateroff", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffWater();
        });

  // water relay on
   server.on("/put/wateron2", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           waterControl2();
        });

 // water relay off
  server.on("/put/wateroff2", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffWater2();
        });

// fan relayd on
   server.on("/put/fanon", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           fanControl();
        });

// fan relay off
   server.on("/put/fanoff", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffFan();
        });


    server.on("/put/heaton", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           heatControl();
        });

// fan relay off
   server.on("/put/heatoff", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffHeat();
        });

  server.on("/put/heaton2", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           heatControl2();
        });

// fan relay off
   server.on("/put/heatoff2", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffHeat2();
        });

server.on("/put/humidon", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           humidControl();
           
        });

// fan relay off
   server.on("/put/humidoff", HTTP_PUT, [](AsyncWebServerRequest *request)
        {
           request->send(200, "text/plain", "");
           turnOffHumidifier();
        });

  
  // User's customized thresholds
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    
    preferences.begin("auto_threshold",false);
 
    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);

        if(p->name() == "soillow")
         {
          preferences.remove(strSoillow); // remove existing key
            //  soilLow = strtof((p->value()).c_str(), NULL);
           preferences.putInt(strSoillow, atoi((p->value()).c_str())); // reinsert new value     
 
         }

       if(p->name() == "soilhigh")
         {
          preferences.remove(strSoilhigh); // remove existing key
          preferences.putInt(strSoilhigh, atoi((p->value()).c_str())); // reinsert new value   
         }

         if(p->name() == "soillow2")
         {
          preferences.remove(strSoillow2); // remove existing key
          preferences.putInt(strSoillow2, atoi((p->value()).c_str())); // reinsert new value     
         }

       if(p->name() == "soilhigh2")
         {
           preferences.remove(strSoilhigh2); // remove existing key
           preferences.putInt(strSoilhigh2, atoi((p->value()).c_str())); // reinsert new value         
         }

         if(p->name() == "templow")
         {
          preferences.remove(strTemplow); // remove existing key
          preferences.putInt(strTemplow, atoi((p->value()).c_str())); // reinsert new value    
         }

        if(p->name() == "temphigh")
         {
          preferences.remove(strTemphigh); // remove existing key
          preferences.putInt(strTemphigh, atoi((p->value()).c_str())); // reinsert new value       
         }

         if(p->name() == "humidhigh")
         {
          preferences.remove(strHumidhigh); // remove existing key
          preferences.putInt(strHumidhigh, atoi((p->value()).c_str())); // reinsert new v
         }

        if(p->name() == "humidlow")
         {
          preferences.remove(strHumidlow); // remove existing key
          preferences.putInt(strHumidlow, atoi((p->value()).c_str())); // reinsert new value    
         }
       }
     preferences.end();
    request->send(200, "text/plain", "");
  });

  server.begin();
}


void taskAutoController(void * parameter)
{
   //controlID sdata;
  uint8_t sreading[2];
  BaseType_t xStatus; /* status of receiving data */
    
 while(1)
  {
    preferences.begin("auto_threshold",false);
    soilLow = preferences.getInt(strSoillow);
    soilHigh = preferences.getInt(strSoilhigh);
    tempLow = preferences.getInt(strTemplow);
    tempHigh = preferences.getInt(strTemphigh);
    humidLow = preferences.getInt(strHumidlow);
    humidHigh = preferences.getInt(strHumidhigh);

    xStatus = xQueueReceive(controlQ, &sreading, pdMS_TO_TICKS(80)); 
     if(xStatus == pdPASS)
    {
       switch(sreading[0])
        {
          case soilS:
            if (sreading[1] <= soilLow) waterControl();
            else if (sreading[1] >= soilHigh) turnOffWater();
            break;
          case tempS:
            if (sreading[1] <= tempLow) {
              heatControl();
              heatControl2();
              }
           else if (sreading[1] >= tempHigh)  {
             turnOffHeat();
             fanControl();
            }
            break;
          case humidS:
            if (sreading[1] <= humidLow) humidControl();
            else if (sreading[1] >= humidHigh) {
              turnOffHumidifier();
              fanControl();
            }
            break;
          case lightS:
            // calculate average full sun hours
            break;
        }
    }
     t.update();
     preferences.end();
  }
    vTaskDelete( NULL );  
}

