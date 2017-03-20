/*
  
  How to send IR code with an Arduino

  This sketch sends IR codes to control devices
  - Samsung TV
  - Sony speaker system 
  - LED Strip with IR receiver
  - and all other IR devices (Working with a simple IR remote control)

  Remote control codes : https://github.com/ibenot/Arduino/blob/master/TP2_IR_RGB_LED_strip/ir_remote_codes.txt
  
  Requirements:
  - 1 IR LED
  - 1 Arduino UNO
  - 1 mini breadboard
  - Jumpers

  | IR LED | Arduino   
  |--------|-------
  | -      | GND       
  | +      | D3 

*/

#include <IRremote.h>

//Required defines for Samsung monitoring
#define SAMSUNG_BITS 32
#define SAMSUNG_HDR_MARK 4500
#define SAMSUNG_HDR_SPACE 4250
#define SAMSUNG_BIT_MARK 600
#define SAMSUNG_ONE_SPACE 1400
#define SAMSUNG_ZERO_SPACE 450

IRsend irsend;

void setup()
{
  //enable serial monitor
  Serial.begin(9600);
}

void loop() {

  //****************
  // Sony speaker
  //****************
  
	for (int i = 0; i < 3; i++) {
		irsend.sendSony(0x6422, 15);
		delay(40);
	}
  Serial.println("Sony: Vol -");
	delay(2000);

  for (int i = 0; i < 3; i++) {
    irsend.sendSony(0x2422, 15);
    delay(40);
  }
  Serial.println("Sony: Vol +");
  delay(2000);

  //****************
  // LED Strip
  //****************

  for (int i = 0; i < 3; i++) {
    irsend.sendNEC(0xFF1AE5, 32);
    delay(40);
  }
  Serial.println(" ");
  Serial.println("LED Strip: Red");
  delay(2000);

  for (int i = 0; i < 3; i++) {
    irsend.sendNEC(0xFF9A65, 32);
    delay(40);
  }
  Serial.println("LED Strip: Green");
  delay(2000);

  for (int i = 0; i < 3; i++) {
    irsend.sendNEC(0xFFA25D, 32);
    delay(40);
  }
  Serial.println("LED Strip: Blue");
  delay(2000);


  //****************
  // Samsung TV
  //****************

  int khz = 38;
  unsigned int irSignal[] = {4500,4400,600,1650,600,1600,600,1600,600,500,600,500,600,550,600,500,600,500,600,1600,600,1650,550,1650,600,500,600,500,600,500,600,500,600,550,550,1650,600,1600,600,1600,600,1650,600,500,600,500,600,500,600,500,600,500,600,550,550,550,600,500,600,1600,600,1600,600,1650,600,1600,600};
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
  Serial.println(" ");
  Serial.println("Samsung TV: Mute");
  delay(2000);

  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
  Serial.println("Samsung TV: Unmute");
  Serial.println(" ");
  delay(2000);
  
}