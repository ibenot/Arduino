/*
  
  How to control 2 RGB LED strips + turn on / off 433MHz power outlet with Cayenne dashboard

  Example for turning on / off Chacon & Zap power outlet with Cayenne dashboard
  Please get your personal remote codes from TP3_433MHz_Receiver and add them to "Chacon power outlet codes" section.
  
  Requirements:
  - 2 RGB 5050 LED Strips
  - 433MHz power outlet (Chacon & Zap)
  - 433MHz transmitter
  - Arduino Ethernet shield or 
  - Wemos D1 Wifi (based on esp8266) or
  - Arduino wifi esp8266 module
  - 9V / 12V external power supply
  - Wago connectors
  - 2 ULN2803

  /!\ Please get your personal remote codes from TP3_433MHz_Receiver.
  /!\ Please insert your custom Token (Your Cayenne account).
  /!\ Please insert your custom SSID + Password (Your wifi credentials).
  
  | 433 Transmitter | Wemos   
  |-----------------|-------
  | VCC             | 5V (Arduino 5V)        
  | GND             | GND       
  | DATA            | D10 

  | ULN2803 1   | Wemos   
  |-------------|----------------------------
  | VIN         | 12V (External power supply)
  | GND         | GND       
  | BLUE        | 3 
  | RED         | 5 
  | GREEN       | 6

  | ULN2803 2   | Wemos   
  |-------------|----------------------------
  | VIN         | 12V (External power supply)
  | GND         | GND       
  | BLUE        | 9 
  | RED         | 10 
  | GREEN       | 11 

  | 433MHz power outlet   | Device   
  |-----------------------|--------------------
  | Zap 1                 | Living room lamp 1
  | Zap 2                 | Kitchen lamp
  | Zap 3                 | Green room lamp
  | Zap 4                 | TV
  | Chacon 2              | Raspberry pi
  | Chacon 3              | Living room lamp 2
  | Chacon 4              | Red room lamp

*/

// Comment this out to disable prints and save space
//#define CAYENNE_PRINT Serial  

#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#include "RCSwitch.h"

char token[] = "XXXXXXXXXXXXXXXX";   // /!\ Enter your Cayenne token here
char ssid[] = "XXXXXXXXXXXXXXXXX";   // /!\ Enter your Wifi name here
char password[] = "XXXXXXXXXXXXX";   // /!\ Enter your Wifi password here

int loadingTimeChaconZap = 20000;   //enable 433MHz 20 seconds after board boot

#define VIRTUAL_PIN_0 V0    //uptime
#define VIRTUAL_PIN_1 V1    //led1 on/off
#define VIRTUAL_PIN_2 V2    //led2 on/off

#define VIRTUAL_PIN_3 V3    //led1 R
#define VIRTUAL_PIN_4 V4    //led1 V
#define VIRTUAL_PIN_5 V5    //led1 B

#define VIRTUAL_PIN_6 V6    //led2 R
#define VIRTUAL_PIN_7 V7    //led2 V
#define VIRTUAL_PIN_8 V8    //led2 B

//Chacon
#define VIRTUAL_PIN_9 V9    //Chacon2
#define VIRTUAL_PIN_10 V10  //Chacon3
#define VIRTUAL_PIN_11 V11  //Chacon4

//Zap
#define VIRTUAL_PIN_12 V12  //Zap1
#define VIRTUAL_PIN_13 V13  //Zap2
#define VIRTUAL_PIN_14 V14  //Zap3
#define VIRTUAL_PIN_15 V15  //Zap4

//All 
#define VIRTUAL_PIN_16 V16  //turn on/off all Chacon + Zap
#define VIRTUAL_PIN_17 V17  //turn on/off all Zap
#define VIRTUAL_PIN_18 V18  //turn on/off all Chacon

//Fade mode
#define VIRTUAL_PIN_19 V19  //turn on/off LED strip fade mode

//Brightness themes to turn on power outlets
#define VIRTUAL_PIN_20 V20  //theme 1: min brightness
#define VIRTUAL_PIN_21 V21  //theme 2: medium brightness
#define VIRTUAL_PIN_22 V22  //theme 3: max brightness

//433MHz
RCSwitch mySwitch = RCSwitch();
int transmit_pin = 1;        //10 for Arduino / 1 for Wemos D1
int pulseLength = 185;       //open TP3_433MHz_Receiver project for more info
int repeatTransmit = 5;
int transmitterDelay = 100;  //Delay between 433 transmitter actions

//LED Strips
int LED_STRIP_1_BLUE  = D9;
int LED_STRIP_1_RED   = D10;
int LED_STRIP_1_GREEN = D11;

int LED_STRIP_2_BLUE  = D3;
int LED_STRIP_2_RED   = D5;
int LED_STRIP_2_GREEN = D6;

// define directions for LED fade
#define UP 0
#define DOWN 1
const int minPWM = 50;            // constants for min PWM
const int maxPWM = 255;           // constants for max PWM
byte fadeDirection = UP;          // State Variable for Fade Direction
int fadeValue = 0;                // Global Fade Value
byte fadeIncrement = 5;           // How smooth to fade?
unsigned long previousFadeMillis; // millis() timing Variable, just for fading
int fadeInterval = 100;           // How fast to increment?
int fademode = 0;                 //set default fademode value to false

/*
 * Setup
 */
 
void setup()
{
  //Serial
  Serial.begin(9600);

  //Led
  pinMode(LED_STRIP_1_RED, OUTPUT);
  pinMode(LED_STRIP_1_GREEN, OUTPUT);
  pinMode(LED_STRIP_1_BLUE, OUTPUT);
  pinMode(LED_STRIP_2_RED, OUTPUT);
  pinMode(LED_STRIP_2_GREEN, OUTPUT);
  pinMode(LED_STRIP_2_BLUE, OUTPUT);
  
  //433MHz Transmitter
  mySwitch.enableTransmit(transmit_pin); 
  // Optional set pulse length.
  mySwitch.setPulseLength(pulseLength);
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(repeatTransmit);

  //LED Strip default fixed to green
  analogWrite(LED_STRIP_1_RED, 0); 
  analogWrite(LED_STRIP_1_GREEN, 255); 
  analogWrite(LED_STRIP_1_BLUE, 0);
  analogWrite(LED_STRIP_2_RED, 0); 
  analogWrite(LED_STRIP_2_GREEN, 255); 
  analogWrite(LED_STRIP_2_BLUE, 0);

  //Cayenne
  Cayenne.begin(token, ssid, password);
}

/*
 * Loop
 */

void loop()
{
  //Run cayenne
  Cayenne.run(); 

  //Allow to execute a task without blocking Cayenne (without delay())
  unsigned long currentMillis = millis();
  if (fademode == 1){
    fadeStrip(currentMillis, 1);
  }
 
}

/*
 * CAYENNE VIRTUAL PIN I/O
 */

//Uptime
CAYENNE_OUT(VIRTUAL_PIN_0){ 
  int uptime = millis() / 1000;
  //Serial.println(" ");
  //Serial.print("Uptime: "); Serial.println(uptime); 
  Cayenne.virtualWrite(VIRTUAL_PIN_0, uptime); 
}

//Led1 on/off
CAYENNE_IN(VIRTUAL_PIN_1)
{
  if(millis() > loadingTimeChaconZap){
    int currentValue = getValue.asInt(); // 0 to 1
    if(currentValue == 1){
      setColorLED(0, 255, 0, 1);
      //Serial.println("LED 1 is green");
    }
    else{
      setColorLED(0, 0, 0, 1);
      //Serial.println("LED 1 is off");
    }
  }
}

//Led2 on/off
CAYENNE_IN(VIRTUAL_PIN_2)
{
  if(millis() > loadingTimeChaconZap){
    int currentValue = getValue.asInt();
    if(currentValue == 1){
      setColorLED(0, 255, 0, 2);
      //Serial.println("LED 2 is green");
    }
    else{
      setColorLED(0, 0, 0, 2);
      //Serial.println("LED 2 is off");
    }
  }
}

//Led1 RED
CAYENNE_IN(VIRTUAL_PIN_3)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble(); // 0 to 255
    currentValue = currentValue / 1023;
    int led1RED = currentValue;  
    setLED1Red(led1RED);
    //Serial.print("LED 1 R = ");
    //Serial.println(led1RED);
  }
}

//Led1 GREEN
CAYENNE_IN(VIRTUAL_PIN_4)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble();
    currentValue = currentValue / 1023;
    int led1GREEN = currentValue;  
    setLED1Green(led1GREEN);
    //Serial.print("LED 1 V = ");
    //Serial.println(led1GREEN);
  }
}

//Led1 BLUE
CAYENNE_IN(VIRTUAL_PIN_5)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble();
    currentValue = currentValue / 1023;
    int led1BLUE = currentValue;  
    setLED1Blue(led1BLUE);
    //Serial.print("LED 1 B = ");
    //Serial.println(led1BLUE);
  }
}

//Led2 RED
CAYENNE_IN(VIRTUAL_PIN_6)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble();
    currentValue = currentValue / 1023;
    int led2RED = currentValue;  
    setLED2Red(led2RED);  // custom red
    //Serial.print("LED 2 R = ");
    //Serial.println(led2RED);
  }
}

//Led2 GREEN
CAYENNE_IN(VIRTUAL_PIN_7)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble();
    currentValue = currentValue / 1023;
    int led2GREEN = currentValue;  
    setLED2Green(led2GREEN);
    //Serial.print("LED 2 V = ");
    //Serial.println(led2GREEN);
  }
}

//Led2 BLUE
CAYENNE_IN(VIRTUAL_PIN_8)
{
  if(millis() > loadingTimeChaconZap){
    float currentValue = getValue.asDouble();
    currentValue = currentValue / 1023;
    int led2BLUE = currentValue;  
    setLED2Blue(led2BLUE);
    //Serial.print("LED 2 B = ");
    //Serial.println(led2BLUE);
  }
}

//Chacon 2 on/off
CAYENNE_IN(VIRTUAL_PIN_9)
{
  if(millis() > loadingTimeChaconZap){
    int currentValue = getValue.asInt(); // 0 to 1
    if(currentValue == 1){
      chacon2_on();
    }
    else{
      chacon2_off();
    }
  }
}

//Chacon 3 on/off
CAYENNE_IN(VIRTUAL_PIN_10)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      chacon3_on();
    }
    else{
      chacon3_off();
    }
  }
}

//Chacon 4 on/off
CAYENNE_IN(VIRTUAL_PIN_11)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      chacon4_on();
    }
    else{
      chacon4_off();
    }
  }
}

//Zap 1 on/off
CAYENNE_IN(VIRTUAL_PIN_12)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      zap1_on();
    }
    else{
      zap1_off();
    }
  }
}

//Zap 2 on/off
CAYENNE_IN(VIRTUAL_PIN_13)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      zap2_on();
    }
    else{
      zap2_off();
    }
  }
}

//Zap 3 on/off
CAYENNE_IN(VIRTUAL_PIN_14)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      zap3_on();
    }
    else{
      zap3_off();
    }
  }
}

//Zap 4 on/off
CAYENNE_IN(VIRTUAL_PIN_15)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      zap4_on();
    }
    else{
      zap4_off();
    }
  }
}

//turn on/off all
CAYENNE_IN(VIRTUAL_PIN_16)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      turn_on_all();
    }
    else{
      turn_off_all();
    }
  }
}

//turn on/off Zaps
CAYENNE_IN(VIRTUAL_PIN_17)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      zap_on();
    }
    else{
      zap_off();
    }
  }
}

//turn on/off Chacons
CAYENNE_IN(VIRTUAL_PIN_18)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      chacon_on();
    }
    else{
      chacon_off();
    }
  }
}

//enable fademode
CAYENNE_IN(VIRTUAL_PIN_19)
{
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1)
    {
      //Serial.println("fademode ON");
      fademode = 1;
    }
    else{
      //Serial.println("fademode OFF");
      fademode = 0;
    }
  }
}

CAYENNE_IN(VIRTUAL_PIN_20) { 
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      set_brightness_min_theme();
    }
  }
}
CAYENNE_IN(VIRTUAL_PIN_21) { 
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      set_brightness_med_theme(); 
    }
  }
}
CAYENNE_IN(VIRTUAL_PIN_22) { 
  if(millis() > loadingTimeChaconZap){
    if(getValue.asInt() == 1){
      set_brightness_max_theme(); 
    }
  }
}

/*
 * LED STRIPS CUSTOM FUNCTIONS
 */

//Custom gradual color transition without delay()
void fadeStrip(unsigned long thisMillis, int ledstripId) {
  // is it time to update yet? if not, nothing happens
  if (thisMillis - previousFadeMillis >= fadeInterval) {
    // yup, it's time!
    if (fadeDirection == UP) {
      fadeValue = fadeValue + fadeIncrement;  
      if (fadeValue >= maxPWM) {
        // At max, limit and change direction
        fadeValue = maxPWM;
        fadeDirection = DOWN;
      }
    } else {
      //if we aren't going up, we're going down
      fadeValue = fadeValue - fadeIncrement;
      if (fadeValue <= minPWM) {
        // At min, limit and change direction
        fadeValue = minPWM;
        fadeDirection = UP;
      }
    }
    // Only need to update when it changes

    analogWrite(LED_STRIP_1_RED, 0); 
    analogWrite(LED_STRIP_1_GREEN, fadeValue); 
    analogWrite(LED_STRIP_1_BLUE, 0);

    analogWrite(LED_STRIP_2_RED, 0); 
    analogWrite(LED_STRIP_2_GREEN, fadeValue); 
    analogWrite(LED_STRIP_2_BLUE, 0);
 
    // reset millis for the next iteration (fade timer only)
    previousFadeMillis = thisMillis;
  }
}

//Set LED strip custom RVB color by strip id
void setColorLED(int red, int green, int blue, int strip)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif

  if (strip == 1){
    analogWrite(LED_STRIP_1_RED, red); 
    analogWrite(LED_STRIP_1_GREEN, green); 
    analogWrite(LED_STRIP_1_BLUE, blue);  
  }

  if (strip == 2){
    analogWrite(LED_STRIP_2_RED, red); 
    analogWrite(LED_STRIP_2_GREEN, green); 
    analogWrite(LED_STRIP_2_BLUE, blue); 
  }
  
}

//Set LED 1 custom Red value 0 - 255
void setLED1Red(int red)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
  #endif
  analogWrite(LED_STRIP_1_RED, red); 
}

//Set LED 1 custom Green value 0 - 255
void setLED1Green(int green)
{
  #ifdef COMMON_ANODE
    green = 255 - green;
  #endif
  analogWrite(LED_STRIP_1_GREEN, green); 
}

//Set LED 1 custom Blue value 0 - 255
void setLED1Blue(int blue)
{
  #ifdef COMMON_ANODE
    blue = 255 - blue;
  #endif
  analogWrite(LED_STRIP_1_BLUE, blue); 
}

//Set LED 2 custom Red value 0 - 255
void setLED2Red(int red)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
  #endif
  analogWrite(LED_STRIP_2_RED, red); 
}

//Set LED 2 custom Green value 0 - 255
void setLED2Green(int green)
{
  #ifdef COMMON_ANODE
    green = 255 - green;
  #endif
  analogWrite(LED_STRIP_2_GREEN, green); 
}

//Set LED 2 custom Blue value 0 - 255
void setLED2Blue(int blue)
{
  #ifdef COMMON_ANODE
    blue = 255 - blue;
  #endif
  analogWrite(LED_STRIP_2_BLUE, blue); 
}

/*
 * 433MHz CUSTOM FUNCTIONS
 */

void chacon2_on(){
  mySwitch.sendTriState("0FFFF0FFFFFF"); delay(transmitterDelay); //Serial.println("Chacon #2: ON"); 
}
void chacon3_on(){
  mySwitch.sendTriState("0FFFFF0FFFFF"); delay(transmitterDelay); //Serial.println("Chacon #3: ON");
}
void chacon4_on(){
  mySwitch.sendTriState("0FFFFFF0FFFF"); delay(transmitterDelay); //Serial.println("Chacon #4: ON");
}
void chacon2_off(){
  mySwitch.sendTriState("0FFFF0FF0000"); delay(transmitterDelay); //Serial.println("Chacon #2: OFF");
}
void chacon3_off(){
  mySwitch.sendTriState("0FFFFF0FFFF0"); delay(transmitterDelay); //Serial.println("Chacon #3: OFF");
}
void chacon4_off(){
  mySwitch.sendTriState("0FFFFFF0FFF0"); delay(transmitterDelay); //Serial.println("Chacon #4: OFF");
}

void zap1_on(){
  mySwitch.sendTriState("F00F0FFF0101"); delay(transmitterDelay); //Serial.println("Zap #1: ON"); 
}
void zap2_on(){
  mySwitch.sendTriState("F00F0FFF1001"); delay(transmitterDelay); //Serial.println("Zap #2: ON"); 
}
void zap3_on(){
  mySwitch.sendTriState("F00F0FF10001"); delay(transmitterDelay); //Serial.println("Zap #3: ON"); 
}
void zap4_on(){
  mySwitch.sendTriState("F00F0F1F0001"); delay(transmitterDelay); //Serial.println("Zap #4: ON"); 
}
void zap1_off(){
  mySwitch.sendTriState("F00F0FFF0110"); delay(transmitterDelay); //Serial.println("Zap #1: OFF");
}
void zap2_off(){
  mySwitch.sendTriState("F00F0FFF1010"); delay(transmitterDelay); //Serial.println("Zap #2: OFF");
}
void zap3_off(){
  mySwitch.sendTriState("F00F0FF10010"); delay(transmitterDelay); //Serial.println("Zap #3: OFF");
}
void zap4_off(){
  mySwitch.sendTriState("F00F0F1F0010"); delay(transmitterDelay); //Serial.println("Zap #4: OFF");
}

//turn on all Zap
void zap_on(){
  if(millis() > loadingTimeChaconZap){
    zap1_on(); zap2_on(); zap3_on(); zap4_on();
  }
}

//turn off all Zap
void zap_off(){
  if(millis() > loadingTimeChaconZap){
    zap1_off(); zap2_off(); zap3_off(); zap4_off();
  }
}

//turn on all Chacon
void chacon_on(){
  if(millis() > loadingTimeChaconZap){
    chacon2_on(); chacon3_on(); chacon4_on();
  }
}

//turn off all Zap
void chacon_off(){
  if(millis() > loadingTimeChaconZap){
    chacon2_off(); chacon3_off(); chacon4_off();
  }
}

//turn on all
void turn_on_all(){
  if(millis() > loadingTimeChaconZap){
    
    //Turn ON Chacon
    chacon4_on(); //chacon2_on(); chacon3_on();
    delay(10);
    
    //Turn ON Zap
    zap1_on(); zap2_on(); zap3_on(); zap4_on();
    delay(10);
    
    //Turn ON LED Strip 1
    setColorLED(0, 255, 0, 1);
    delay(10);

    //Turn ON LED Strip 2
    setColorLED(0, 255, 0, 2);
  }
}

//turn off all
void turn_off_all(){
  if(millis() > loadingTimeChaconZap){

    //Turn OFF Chacon
    chacon4_off(); //chacon2_off(); chacon3_off();
    delay(10);

    //Turn OFF Zap
    zap1_off(); zap2_off(); zap3_off(); zap4_off();
    delay(10);

    //Turn OFF LED Strip 1
    setColorLED(0, 0, 0, 1);
    delay(10);

    //Turn OFF LED Strip 2
    setColorLED(0, 0, 0, 2);
  }
}

//turn on "min brightness" theme 1
void set_brightness_min_theme(){
  if(millis() > loadingTimeChaconZap){

    //OFF
    chacon4_off();
    delay(100);

    zap3_off();
    delay(100);

    setColorLED(0, 0, 0, 1);
    delay(100);

    setColorLED(0, 0, 0, 2);
    delay(100);

    //ON
    zap1_on(); zap2_on();
  }  
}

//turn on "med brightness" theme 2
void set_brightness_med_theme(){
  if(millis() > loadingTimeChaconZap){

    //OFF
    chacon4_off();
    delay(100);

    //ON
    zap1_on(); zap2_on(); zap3_on();

    setColorLED(0, 255, 0, 1);
    delay(100);

    setColorLED(0, 255, 0, 2);
    delay(100);
  }  
}

//turn on "max brightness" theme 3
void set_brightness_max_theme(){
  if(millis() > loadingTimeChaconZap){

    //ON
    zap1_on(); zap2_on(); zap3_on();
    delay(100);

    chacon4_on();
    delay(100);

    setColorLED(0, 255, 0, 1);
    delay(100);

    setColorLED(0, 255, 0, 2);
    delay(100);
  }  
}
