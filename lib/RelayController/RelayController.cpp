#include "RelayController.h"
Timer t;
// Controller PIN
#define waterRelayPin 19      //Water valve relay
#define waterRelayPin2 23     //Water valve relay2

#define heatRelayPin 16      //Heater relay     
#define fanRelayPin 17      //Fan relay   
#define humidRelayPin 5   //Humidifier
#define heatRelayPin2 18 // heater 2

// LED PIN
#define redLedPin 27
#define whiteLedPin 25
#define yellowLedPin 26

 const unsigned long humidOnTime = 30000 ; // Change to 30 minutes
 const unsigned long fanOnTime = 30000 ; // Change to 30 minutes
 const unsigned long heatOnTime = 30000 ; // Change to 30 minutes
 const unsigned long waterOnTime = 30000 ; // Change to 15 minutes

void setupPinMode(void)
{
   // LED pins
  pinMode(whiteLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

 // controller Relay pins
  pinMode(waterRelayPin, OUTPUT); //water relay
  pinMode(waterRelayPin2, OUTPUT); // 2nd water relay
  pinMode(heatRelayPin, OUTPUT); // heater relay
  pinMode(heatRelayPin2, OUTPUT); // heater relay
  pinMode(fanRelayPin, OUTPUT); // fan relay
  pinMode(humidRelayPin, OUTPUT);// humidifier

  digitalWrite(waterRelayPin, HIGH); 
  digitalWrite(waterRelayPin2, HIGH); 
  digitalWrite(heatRelayPin, HIGH); 
  digitalWrite(heatRelayPin2, HIGH); 
  digitalWrite(fanRelayPin, HIGH); 
  digitalWrite(humidRelayPin, HIGH); 
}

// turn on/off controller LED
void blinkLed(int ledPin, int ledState)
{
  digitalWrite(ledPin, ledState);
}

void turnOffWater()
{
  int pinState = digitalRead(waterRelayPin);
 
  if (pinState == LOW)
    {
       Serial.println("water relay OFF" ); 
      
       digitalWrite(waterRelayPin, HIGH); // turn off controler RELAY
    }

   int ledState =  digitalRead(whiteLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(whiteLedPin, LOW); // turn off LED
      }
}

void turnOffWater2()
{
  int pinState = digitalRead(waterRelayPin2);
 
  if (pinState == LOW)
    {
       Serial.println("water relay OFF" ); 
      
       digitalWrite(waterRelayPin2, HIGH); // turn off controler RELAY
    }

  /*  int ledState =  digitalRead(whiteLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(whiteLedPin, LOW); // turn off LED
      } */
}


void turnOffHeat()
{
  int pinState = digitalRead(heatRelayPin);

  if (pinState == LOW)
    {
       Serial.println("Heat relay OFF" ); 
      
       digitalWrite(heatRelayPin, HIGH); // turn off controler RELAY
    }

   int ledState =  digitalRead(redLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(redLedPin, LOW); // turn off LED
      }
}

void turnOffHeat2()
{
  int pinState = digitalRead(heatRelayPin2);

  if (pinState == LOW)
    {
       Serial.println("Heat relay OFF" ); 
      
       digitalWrite(heatRelayPin2, HIGH); // turn off controler RELAY
    }
}

void turnOffFan()
{
  int pinState = digitalRead(fanRelayPin);

  if (pinState == LOW)
    {
       Serial.println("Fan relay OFF" ); 
        digitalWrite(fanRelayPin, HIGH); // turn off controler RELAY
    }

   int ledState =  digitalRead(yellowLedPin); 
      if (ledState == HIGH)
      {
        blinkLed(yellowLedPin, LOW); // turn off LED
      }
 }

 void turnOffHumidifier()
{
  int pinState = digitalRead(humidRelayPin);

  if (pinState == LOW)
    {
       Serial.println("Humidifier relay OFF" ); 
        digitalWrite(humidRelayPin, HIGH); // turn off controler RELAY
    }
 }

//Turn on water valve
void waterControl()
{
  
  int pinState = digitalRead(waterRelayPin);
  int ledState = digitalRead(whiteLedPin);
 
  if (pinState != LOW)
    {
      Serial.println("water relay ON" ); 
       digitalWrite(waterRelayPin, LOW); 
      
      if (ledState != HIGH)
      {
        blinkLed(whiteLedPin, HIGH);
      }
   
     t.after(waterOnTime, turnOffWater);
   }
}

//Turn on water valve
void waterControl2()
{
  
  int pinState = digitalRead(waterRelayPin2);
  //int ledState = digitalRead(whiteLedPin);
 
  if (pinState != LOW)
    {
      Serial.println("water relay2 ON" ); 
       digitalWrite(waterRelayPin2, LOW); 
      
    /*   if (ledState != HIGH)
      {
        blinkLed(whiteLedPin, HIGH);
      } */
   
     t.after(waterOnTime, turnOffWater);
   }
}

void heatControl()
{
  int pinState = digitalRead(heatRelayPin);
  int ledState = digitalRead(redLedPin);
 
  if (pinState != LOW)
    {
      Serial.println("Heater relay ON" ); 
       digitalWrite(heatRelayPin, LOW); 
      
      if (ledState != HIGH)
      {
        blinkLed(redLedPin, HIGH);
      }
   
       t.after(heatOnTime, turnOffHeat);
    }
}

void heatControl2()
{

  int pinState = digitalRead(heatRelayPin2);

 
  if (pinState != LOW)
    {
      Serial.println("Heater2 relay ON" ); 
       digitalWrite(heatRelayPin2, LOW); 
       t.after(heatOnTime, turnOffHeat2);
    }
}

void fanControl()
{
  
  int pinState = digitalRead(fanRelayPin);
  int ledState = digitalRead(yellowLedPin);
 
  if (pinState != LOW)
    {
      Serial.println("fan relay ON" ); 
       digitalWrite(fanRelayPin, LOW); 
      
      if (ledState != HIGH)
      {
        blinkLed(yellowLedPin, HIGH);
      }
       t.after(fanOnTime, turnOffFan);    
    }
} 

void humidControl()
{
    int pinState = digitalRead(humidRelayPin);
 // int ledState = digitalRead(yellowLedPin);
 
  if (pinState != LOW)
    {
      Serial.println("Humidifier relay ON" ); 
       digitalWrite(humidRelayPin, LOW);      
       t.after(humidOnTime, turnOffHumidifier);    
    }
}