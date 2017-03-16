/*
  
  How to control 1 RGB LED strip with IR remote.
  
  /!\ Open your serial monitor and press any key of your IR Remote. 
  
  Requirements:
    - Arduino UNO R3
    - IR Receiver + IRremote library
    - IR Remote

  | IR Receiver     | Arduino   
  |-----------------|-------
  | VCC             | 5V (Arduino 5V)         
  | GND             | GND       
  | DATA            | 8 

  | ULN2803     | Arduino   
  |-------------|----------------------------
  | VIN         | 9V (External power supply)
  | GND         | GND       
  | BLUE        | 9 
  | RED         | 10 
  | GREEN       | 11 

*/

#include <IRremote.h>

//IR
int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;

//LED Strip
const int green = 11;
const int red = 10;
const int blue = 9;

void setup()
{
  Serial.begin(9600);

  //set output
  pinMode (green,OUTPUT);
  pinMode (red,OUTPUT);
  pinMode (blue,OUTPUT); 

  //set default color
  digitalWrite(green,HIGH);

  //init IR
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results)) {
    
    //Get value
    Serial.println(results.value, HEX);

    // /!\ Replace results.value with your custom remote codes. 
    
    //Red
    if(results.value == 0xFF1AE5){
      digitalWrite(red,HIGH);
      digitalWrite(green,LOW);
      digitalWrite(blue,LOW);
    }
    //Blue
    else if(results.value == 0xFFA25D){
      digitalWrite(red,LOW);
      digitalWrite(green,LOW);
      digitalWrite(blue,HIGH);
    }
    //Green 0xFF9A65
    else{
      digitalWrite(red,LOW);
      digitalWrite(green,HIGH);
      digitalWrite(blue,LOW);
    }
    
    //IR resume
    irrecv.resume();
        
  }
}
