/*
  
  Example for receiving 433MHz codes from Chacon & Zap remotes using rc-switch library.

  /!\ Open your serial monitor and press any key of your RF Remote. 
  
  | Receiver   | Arduino
  |------------|---------
  | VCC        | 5V
  | GND        | GND
  | DATA       | 2 (digital)
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  //enable serial monitor
  Serial.begin(9600);
  // Receiver on interrupt 0 => that is pin Digital #2 (Arduino UNO R3)
  mySwitch.enableReceive(0); 
  // Optional set pulse length.
  mySwitch.setPulseLength(185);
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(5);
}

void loop() {
  if (mySwitch.available()) {
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
}
