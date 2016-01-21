/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// printing string of characters and values
#include <stdio.h>
int l = 0;

// constants won't change. They're used here to 
// set pin numbers:
const int pinButton1 = 10;     // the number of the pushbutton pin
const int pinButton2 = 9;
const int pinButton3 = 8;
const int pinLed =  13;      // the number of the LED pin

// variables will change:
int stateButton1 = 0;         // variable for reading the pushbutton status
int stateButton2 = 0; 
int stateButton3 = 0;

/*
int previousStateButton1 = 1;         
int previousStateButton2 = 1; 
int previousStateButton3 = 1;

int possibleStateButton1 = 0;         
int possibleStateButton2 = 0; 
int possibleStateButton3 = 0;
*/

boolean buttonPressed = false;
unsigned long timeButtonPressed = 0;
const int precision = 150; // (ms)

int counter = 0;
int counterButton1 = 0;
int counterButton2 = 0;
//int counterButton3 = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(pinLed, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT); 
  Serial.begin(9600); 
  Serial.print("START:\n");
  char buffer [50];
    l=sprintf(buffer, "%d: %d, %d\n", counter, counterButton1, counterButton2);
    for(int i= 0; i<l; i++) 
      Serial.print(buffer[i]); 
}

void loop(){
  // read the state of the pushbutton value:

  if( ( millis() - timeButtonPressed ) > precision ){
      timeButtonPressed = millis();
      stateButton1 = digitalRead(pinButton1);
      stateButton2 = digitalRead(pinButton2);
      stateButton3 = digitalRead(pinButton3);
  

    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if( buttonPressed == false && stateButton1 == HIGH ) {  
      // turn LED on:    
      digitalWrite(pinLed, HIGH);
      counter++;
      counterButton1 = (counterButton1 + 1) % 10;
      buttonPressed == true;
      
      char buffer [50];
      l=sprintf(buffer, "%d: %d, %d\n", counter, counterButton1, counterButton2);
      for(int i= 0; i<l; i++) 
        Serial.print(buffer[i]);  
    } 

    if( buttonPressed == false && stateButton2 == HIGH ) {  
      // turn LED on:    
      digitalWrite(pinLed, HIGH);
      counter++;
      counterButton2 = ( counterButton2 + 1 ) % 10;
      buttonPressed == true;
      
      char buffer [50];
      l=sprintf(buffer, "%d: %d, %d\n", counter, counterButton1, counterButton2);
      for(int i= 0; i<l; i++) 
        Serial.print(buffer[i]);  
    } 

    if( buttonPressed == false && stateButton3 == HIGH ) {  
      // turn LED on:    
      digitalWrite(pinLed, HIGH);
      counter++;
      counterButton2 = ( counterButton2 + 9 ) % 10;
      buttonPressed == true;
      
      char buffer [50];
      l=sprintf(buffer, "%d: %d, %d\n", counter, counterButton1, counterButton2);
      for(int i= 0; i<l; i++) 
        Serial.print(buffer[i]);  
    } 
    
    
    if( stateButton1 == LOW && stateButton2 == LOW && stateButton3 == LOW ){
      // turn LED off:
      digitalWrite(pinLed, LOW); // buttonState == 0
      buttonPressed = false;
    }
  }
  
}
