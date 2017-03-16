/*
  
  How to control 2 RGB LED strips with an Arduino UNO R3.
    
  Requirements:
    - Arduino UNO R3
    - ULN2803 x 2

  | ULN2803 1   | Arduino   
  |-------------|----------------------------
  | VIN         | 12V (External power supply)
  | GND         | GND       
  | BLUE        | 3 
  | RED         | 5 
  | GREEN       | 6 

  | ULN2803 2   | Arduino   
  |-------------|----------------------------
  | VIN         | 12V (External power supply)
  | GND         | GND       
  | BLUE        | 9 
  | RED         | 10 
  | GREEN       | 11 

*/

//LED strip 1
int LED_STRIP_1_BLUE  = 3;
int LED_STRIP_1_RED   = 5;
int LED_STRIP_1_GREEN = 6;

//LED strip 2
int LED_STRIP_2_BLUE  = 9;
int LED_STRIP_2_RED   = 10;
int LED_STRIP_2_GREEN = 11;

int sleep = 1500;

void setup()
{
  Serial.begin(9600);
  
  pinMode(LED_STRIP_1_RED, OUTPUT);
  pinMode(LED_STRIP_1_GREEN, OUTPUT);
  pinMode(LED_STRIP_1_BLUE, OUTPUT);

  pinMode(LED_STRIP_2_RED, OUTPUT);
  pinMode(LED_STRIP_2_GREEN, OUTPUT);
  pinMode(LED_STRIP_2_BLUE, OUTPUT);
}

void loop()
{  
  setColor(255, 0, 0);  // red
  Serial.println("LED strip is RED");
  delay(sleep);
  
  setColor(0, 255, 0);  // green
  Serial.println("LED strip is GREEN");
  delay(sleep);
  
  setColor(0, 0, 255);  // blue
  Serial.println("LED strip is BLUE");
  delay(sleep);
  
  setColor(255, 255, 0);  // yellow
  Serial.println("LED strip is YELLOW");
  delay(sleep);  
  
  setColor(80, 0, 80);  // purple
  Serial.println("LED strip is PURPLE");
  delay(sleep);
  
  setColor(0, 255, 255);  // aqua
  Serial.println("LED strip is AQUA");
  delay(sleep);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  
  analogWrite(LED_STRIP_1_RED, red);
  analogWrite(LED_STRIP_1_GREEN, green);
  analogWrite(LED_STRIP_1_BLUE, blue); 

  analogWrite(LED_STRIP_2_RED, red);
  analogWrite(LED_STRIP_2_GREEN, green);
  analogWrite(LED_STRIP_2_BLUE, blue);

  Serial.println(" ");
  Serial.print("red: ");
  Serial.println(red);

  Serial.print("green: ");
  Serial.println(green);

  Serial.print("blue: ");
  Serial.println(blue);
}
