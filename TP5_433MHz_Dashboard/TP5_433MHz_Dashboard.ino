/*
  
  Example for turning on / off Chacon & Zap power outlet with Cayenne dashboard
  Please get your personal remote codes from TP3_433MHz_Receiver and add them to "Chacon power outlet codes" section.
  
  Requirements:
    - Arduino Ethernet shield or 
    - Wemos D1 Wifi (based on esp8266) or
    - Arduino wifi esp8266 module

  /!\ Please get your personal remote codes from TP3_433MHz_Receiver.
  /!\ Please insert your custom Token (Your Cayenne account).
  /!\ Please insert your custom SSID + Password (Your wifi credentials).

  | Transmitter | Arduino   | Wemos D1
  |-------------|-----------|----------
  | VCC         | 5V        | 5V
  | GND         | GND       | GND
  | DATA        | D10       | D1
  
*/

//#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial    // Comment this out to disable prints and save space

#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"

//Wifi
#include "CayenneWiFiClient.h"
// or Ethernet
//#include <CayenneEthernet.h>

#include <RCSwitch.h>
int i=0;
int sleep = 100;
int transmit_pin = 1;
int pulseLength = 185;
int repeatTransmit = 5;

char token[] = "XXXXXXXXXXXXXXXX";   // /!\ Enter your Cayenne token here
char ssid[] = "XXXXXXXXXXXXXXXXX";   // /!\ Enter your Wifi name here
char password[] = "XXXXXXXXXXXXX";   // /!\ Enter your Wifi password here

// Chacon power outlet /!\ Get your remote codes from TP3_433MHz_Receiver project
char chacon2_on[]  = "0FFFF0FFFFFF";
char chacon3_on[]  = "0FFFFF0FFFFF";
char chacon4_on[]  = "0FFFFFF0FFFF";
char chacon2_off[] = "0FFFF0FF0000";
char chacon3_off[] = "0FFFFF0FFFF0";
char chacon4_off[] = "0FFFFFF0FFF0";

// RC-Switch
RCSwitch mySwitch = RCSwitch();

// Cayenne 433MHZ Virtual Pins
#define VIRTUAL_PIN_0 V0 // Virtual pin to led 13 for debug
#define VIRTUAL_PIN_5 V5 // Virtual pin to manage Chacon 2
#define VIRTUAL_PIN_6 V6 // Virtual pin to manage Chacon 3
#define VIRTUAL_PIN_7 V7 // Virtual pin to manage Chacon 4
#define VIRTUAL_PIN_8 V8 // Virtual pin to manage all Chacon power outlet

void setup() {
  //Debug
  Serial.begin(9600);
  
  //433MHz
  mySwitch.enableTransmit(transmit_pin);
  mySwitch.setPulseLength(pulseLength);
  mySwitch.setRepeatTransmit(repeatTransmit);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);delay(200); digitalWrite(LED_BUILTIN, HIGH);delay(200); 
  digitalWrite(LED_BUILTIN, LOW);delay(200); digitalWrite(LED_BUILTIN, HIGH);

  //Cayenne
  Cayenne.begin(token, ssid, password); 
}

void loop() {
  Cayenne.run();
}

CAYENNE_IN(VIRTUAL_PIN_0)
{
  // get value sent from Cayenne dashboard
  int currentValue = getValue.asInt(); // 0 to 1

  if(currentValue == 1){
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.println("LED 13: ON");
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH); 
    Serial.println("LED 13: OFF");
  }

}

CAYENNE_IN(VIRTUAL_PIN_5)
{
  // get value sent from Cayenne dashboard
  int currentValue = getValue.asInt(); // 0 to 1

  if(currentValue == 1){
    //Chacon ON 2
    mySwitch.sendTriState(chacon2_on); delay(sleep); 
    Serial.println("CHACON 2: ON");
  }
  else{
    //Chacon OFF 2
    mySwitch.sendTriState(chacon2_off); delay(sleep);
    Serial.println("CHACON 2: OFF");
  }

}

CAYENNE_IN(VIRTUAL_PIN_6)
{
  if(getValue.asInt() == 1){
    //Chacon ON 3
    mySwitch.sendTriState(chacon3_on); delay(sleep); 
    Serial.println("CHACON 3: ON");
  }
  else{
    //Chacon OFF 3
    mySwitch.sendTriState(chacon3_off); delay(sleep);
    Serial.println("CHACON 3: OFF");
  }

}

CAYENNE_IN(VIRTUAL_PIN_7)
{
  if(getValue.asInt() == 1){
    //Chacon ON 4
    mySwitch.sendTriState(chacon4_on); delay(sleep); 
    Serial.println("CHACON 4: ON");
  }
  else{
    //Chacon OFF 4
    mySwitch.sendTriState(chacon4_off); delay(sleep);
    Serial.println("CHACON 4: OFF");
  }

}

CAYENNE_IN(VIRTUAL_PIN_8)
{
  if(getValue.asInt() == 1){
    //Turn ON all Chacon power outlet in one clic
    mySwitch.sendTriState(chacon2_on);
    mySwitch.sendTriState(chacon3_on);
    mySwitch.sendTriState(chacon4_on);
    delay(sleep); 
    Serial.println("CHACON All: ON");
  }
  else{
    //Turn OFF all Chacon power outlet in one clic
    mySwitch.sendTriState(chacon2_off);
    mySwitch.sendTriState(chacon3_off);
    mySwitch.sendTriState(chacon4_off);
    delay(sleep);
    Serial.println("CHACON All: OFF");
  }

}
