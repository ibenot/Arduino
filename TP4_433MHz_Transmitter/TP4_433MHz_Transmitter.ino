/*
  
  Example for sending code from Arduino to Chacon & Zap power outlet
    
  | Transmitter | Arduino   | Wemos D1
  |-------------|-----------|----------
  | VCC         | 5V        | 5V
  | GND         | GND       | GND
  | DATA        | D10       | D1

*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

int sleep = 1000;
int transmit_pin = 1; //10 for Arduino / 1 for Wemos D1
int pulseLength = 185;
int repeatTransmit = 5;

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(transmit_pin);
  mySwitch.setPulseLength(pulseLength);
  mySwitch.setRepeatTransmit(repeatTransmit);
}

void loop() {
    
  //-- Chacon power outlet --

  //ON 2
  mySwitch.sendTriState("0FFFF0FFFFFF"); Serial.println("Chacon #2: ON"); delay(sleep); 
  //ON 3
  mySwitch.sendTriState("0FFFFF0FFFFF"); Serial.println("Chacon #3: ON"); delay(sleep); 
  //ON 4
  mySwitch.sendTriState("0FFFFFF0FFFF"); Serial.println("Chacon #4: ON"); delay(sleep);

  //OFF 2
  mySwitch.sendTriState("0FFFF0FF0000"); Serial.println("Chacon #2: OFF"); delay(sleep);
  //OFF 3 
  mySwitch.sendTriState("0FFFFF0FFFF0"); Serial.println("Chacon #3: OFF"); delay(sleep);
  //OFF 4 
  mySwitch.sendTriState("0FFFFFF0FFF0"); Serial.println("Chacon #4: OFF"); delay(sleep);

  //-- Zap power outlet --

  //ON 1
  mySwitch.sendTriState("F00F0FFF0101"); Serial.println("Zap #1: ON"); delay(sleep);
  //ON 2
  mySwitch.sendTriState("F00F0FFF1001"); Serial.println("Zap #2: ON"); delay(sleep);
  //ON 3
  mySwitch.sendTriState("F00F0FF10001"); Serial.println("Zap #3: ON"); delay(sleep);
  //ON 4
  mySwitch.sendTriState("F00F0F1F0001"); Serial.println("Zap #4: ON"); delay(sleep);
  
  //OFF 1
  mySwitch.sendTriState("F00F0FFF0110"); Serial.println("Zap #1: OFF"); delay(sleep);
  //OFF 2
  mySwitch.sendTriState("F00F0FFF1010"); Serial.println("Zap #2: OFF"); delay(sleep);
  //OFF 3
  mySwitch.sendTriState("F00F0FF10010"); Serial.println("Zap #3: OFF"); delay(sleep);
  //OFF 4
  mySwitch.sendTriState("F00F0F1F0010"); Serial.println("Zap #4: OFF"); delay(sleep);

}
