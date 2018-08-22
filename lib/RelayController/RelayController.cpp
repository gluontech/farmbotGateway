#include "RelayController.h"
Timer t;
// Controller PIN
#define waterRelayPin 16      //Water valve relay
#define heatRelayPin  5      //Heater relay     
#define fanRelayPin 17      //Fan relay   
// LED PIN
#define redLedPin 19
#define whiteLedPin 13
#define yellowLedPin 18

void setupPinMode(void)
{
   // LED pins
  pinMode(whiteLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

 // controller Relay pins
  pinMode(waterRelayPin, OUTPUT); //water relay
  pinMode(heatRelayPin, OUTPUT); // heater relay
  pinMode(fanRelayPin, OUTPUT); // fan relay
}

// turn on/off controller LED
void blinkLed(int ledPin, int ledState)
{
  digitalWrite(ledPin, ledState);
}

void turnOffWater()
{
  int pinState = digitalRead(waterRelayPin);
 // int pinState = digitalRead(WATER_PIN);
  if (pinState == HIGH)
    {
       Serial.println("water relay OFF" ); 
      
       digitalWrite(waterRelayPin, LOW); // turn off controler RELAY
    }

   int ledState =  digitalRead(whiteLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(whiteLedPin, LOW); // turn off LED
      }
}

void turnOffHeat()
{
  int pinState = digitalRead(heatRelayPin);
 // int pinState = digitalRead(WATER_PIN);
  if (pinState == HIGH)
    {
       Serial.println("Heat relay OFF" ); 
      
       digitalWrite(heatRelayPin, LOW); // turn off controler RELAY
    }

   int ledState =  digitalRead(redLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(redLedPin, LOW); // turn off LED
      }
}

void turnOffFan()
{
  int pinState = digitalRead(fanRelayPin);
 // int pinState = digitalRead(WATER_PIN);
  if (pinState == HIGH)
    {
       Serial.println("Fan relay OFF" ); 
        digitalWrite(fanRelayPin, LOW); // turn off controler RELAY
    }

   int ledState =  digitalRead(yellowLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(yellowLedPin, LOW); // turn off LED
      }
 }

//Turn on water valve
void waterControl()
{
  const unsigned long waterOnTime = 30000 ; // Change to 15 minutes
  int pinState = digitalRead(waterRelayPin);
  int ledState = digitalRead(whiteLedPin);
 
  if (pinState != HIGH)
    {
      Serial.println("water relay ON" ); 
       digitalWrite(waterRelayPin, HIGH); 
      
      if (ledState != HIGH)
      {
        blinkLed(whiteLedPin, HIGH);
      }
   
     t.after(waterOnTime, turnOffWater);
   }
}

void heatControl()
{
  const unsigned long heatOnTime = 30000 ; // Change to 30 minutes
  int pinState = digitalRead(heatRelayPin);
  int ledState = digitalRead(redLedPin);
 
  if (pinState != HIGH)
    {
      Serial.println("Heater relay ON" ); 
       digitalWrite(heatRelayPin, HIGH); 
      
      if (ledState != HIGH)
      {
        blinkLed(redLedPin, HIGH);
      }
   
       t.after(heatOnTime, turnOffHeat);
    }
}

void fanControl()
{
  const unsigned long fanOnTime = 30000 ; // Change to 30 minutes
  int pinState = digitalRead(fanRelayPin);
  int ledState = digitalRead(yellowLedPin);
 
  if (pinState != HIGH)
    {
      Serial.println("fan relay ON" ); 
       digitalWrite(fanRelayPin, HIGH); 
      
      if (ledState != HIGH)
      {
        blinkLed(yellowLedPin, HIGH);
      }
       t.after(fanOnTime, turnOffFan);    
    }
} 

void humidControl()
{
  Serial.println("Humidity control on" ); 
}