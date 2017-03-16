/*
  
  How to make a Flower Power like (http://global.parrot.com/au/products/flower-power/)

  This Flower Power monitors and analyzes the four parameters essential to your plant's health:
  - Temperature
  - Brightness
  - Soil moisture
  - Humidity
  
  Requirements:
  - 1 LED
  - 1 photoresistor
  - 1 DHT11
  - 1 water sensor for soil moisture
  - 9V external power supply
  - 1 Arduino UNO
  - 1 Ethernet shield
  - 2 mini breadboard
  - 2 resistors
  - Jumpers

  | LED   | Arduino   
  |-------|-------
  | VCC   | 5V       
  | GND   | GND       
  | DATA  | D2 
  
  | DHT11 | Arduino   
  |-------|-------
  | VCC   | 5V       
  | GND   | GND       
  | DATA  | A1 

  | Water sensor | Arduino   
  |--------------|-------
  | VCC          | 5V      
  | GND          | GND       
  | DATA         | A2 

  | Photoresistor | Arduino   
  |---------------|-------
  | VCC           | 5V        
  | GND           | GND       
  | DATA          | A0 

*/

#include <CayenneEthernet.h>
#include <dht11.h>

//CAYENNE CONFIG
#define CAYENNE_PRINT Serial
char token[] = "XXXXXXXXXXXXXXXX";   // /!\ Enter your Cayenne token here

//LED
const int LED_PIN = 2;

//BRIGHTNESS
#define BRIGHTNESS_PIN A0

//HUMIDITY + TEMPERATURE
#define DHT11_PIN A1
dht11 DHT11;

//WATER SENSOR
#define WATER_SENSOR_PIN A2

//CAYENNE VIRTUAL PINS
#define VIRTUAL_PIN_1 V1   //temperature
#define VIRTUAL_PIN_2 V2   //uptime
#define VIRTUAL_PIN_3 V3   //water sensor
#define VIRTUAL_PIN_4 V4   //humidity
#define VIRTUAL_PIN_12 V12 //brightness

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Cayenne.begin(token);
}

void loop() {
  Cayenne.run();
}

float getUptime() {
  delay(100);
  float uptime = millis() / 1000;
  Serial.print("Uptime = ");
  Serial.print(uptime);
  Serial.println(" sec ");
  return uptime;
}

int getWaterLevel(){
  delay(100);
  int water = analogRead(WATER_SENSOR_PIN);
  Serial.print("Water level = ");
  Serial.println(water);

  //turn on/off led depending water sensor value
  if(water <= 10){
    digitalWrite(LED_PIN, HIGH);
  }
  else if(water > 10 && water <= 100){
    digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);  delay(100);
    digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);  delay(100);
    digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);  delay(100);
    digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW);  delay(100);
    digitalWrite(LED_PIN, HIGH);
  }
  else if(water > 100 && water <= 300){
    digitalWrite(LED_PIN, HIGH); delay(500); digitalWrite(LED_PIN, LOW);  delay(500);
    digitalWrite(LED_PIN, HIGH); delay(500); digitalWrite(LED_PIN, LOW);  delay(500);
    digitalWrite(LED_PIN, HIGH);
  }
  else{
    digitalWrite(LED_PIN, LOW);
  }
  
  return water;
}

float getHumidity() {
  delay(100);
  DHT11.read(DHT11_PIN);
  float humidity = (float)DHT11.humidity;
  Serial.print("Humidity = ");
  Serial.println(humidity);
  return humidity;
}

float getTemperature() {
  delay(100);
  DHT11.read(DHT11_PIN);
  float temperature = (float)DHT11.temperature;
  Serial.println(" ");
  Serial.print("Temperature = ");
  Serial.println(temperature);
  Serial.print("Brightness = ");
  Serial.println(analogRead(BRIGHTNESS_PIN));
  return temperature;
}

float getBrightness(){
  delay(100);
  int brightness = analogRead(BRIGHTNESS_PIN);  
  return brightness;
}

//Temperature
CAYENNE_OUT(VIRTUAL_PIN_1){ Cayenne.virtualWrite(VIRTUAL_PIN_1, getTemperature(), TEMPERATURE, CELSIUS); }

//Uptime
CAYENNE_OUT(VIRTUAL_PIN_2){ Cayenne.virtualWrite(VIRTUAL_PIN_2, getUptime()); }

//Water level
CAYENNE_OUT(VIRTUAL_PIN_3){ Cayenne.virtualWrite(VIRTUAL_PIN_3, getWaterLevel()); }

//Humidity
CAYENNE_OUT(VIRTUAL_PIN_4){ Cayenne.virtualWrite(VIRTUAL_PIN_4, getHumidity()); }

//Brightness
CAYENNE_OUT(VIRTUAL_PIN_12){ Cayenne.virtualWrite(VIRTUAL_PIN_12, getBrightness()); }

