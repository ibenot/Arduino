# Tuto 1: How to control two RGB LED strips ?

## Demo

How to control two RGB LED strips with an Arduino and show results in Arduino IDE serial monitor.

![How to control two RGB LED strips ?](overview.gif)
    
## Requirements
- 1 Arduino UNO
- 2 x ULN2803
- 2 RGB 5050 LED strips
- 2 RGB LED strip connectors
- 1 12V external power supply
- Jumpers / Breadboard

## Wiring

| ULN2803 1   | Arduino   
|-------------|----------------------------
| VIN         | 12V (External power supply)
| GND         | GND       
| BLUE        | D3 
| RED         | D5 
| GREEN       | D6 

| ULN2803 2   | Arduino   
|-------------|----------------------------
| VIN         | 12V (External power supply)
| GND         | GND       
| BLUE        | D9 
| RED         | D10 
| GREEN       | D11 


