//------------BUTTONS-------------

// printing string of characters and values
#include <stdio.h>
int l = 0;

const int pinButton1 = 9;    
const int pinButton2 = 10;
const int pinButton3 = 11;
const int pinLed =  13;   

int stateButton1 = 0;       
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

//------------MENU----------------

int level = 0;
int val = 0;

String incomingString;
int menuSelection = 0;
int previousMenu = -1;

int value[] = {0,0,0,0};

//--------------------------------

void setup() {

  // initialize the LED pin as an output:
  pinMode(pinLed, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT); 

  Serial.begin(9600);
  Serial.println("\ninitializing...");
}

void loop() {

  // Reading button/text value
  /*if(Serial.available() > 0){
    char ch = Serial.read();
    
    if(ch != 10){
      incomingString += ch;
    }else{
      val = incomingString.toInt();
      
      incomingString = "";
      
    }
  }*/

  // read the state of the pushbutton value:
  if( ( millis() - timeButtonPressed ) > precision ){
      timeButtonPressed = millis();
      stateButton1 = digitalRead(pinButton1);
      stateButton2 = digitalRead(pinButton2);
      stateButton3 = digitalRead(pinButton3);

      // check if the pushbutton is pressed.
      // if it is, the buttonState is HIGH:
      if( buttonPressed == false && stateButton1 == HIGH ) {      
        digitalWrite(pinLed, HIGH);
        val = 1;
        buttonPressed == true;  
      } 

      if( buttonPressed == false && stateButton2 == HIGH ) {     
        digitalWrite(pinLed, HIGH);
        val = -1;
        buttonPressed == true;  
      } 

      if( buttonPressed == false && stateButton3 == HIGH ) {      
        digitalWrite(pinLed, HIGH);
        level = (level + 1)%2;
        Serial.print("Level: ");
        Serial.println(level);
        buttonPressed == true; 
      } 
    
    
      if( stateButton1 == LOW && stateButton2 == LOW && stateButton3 == LOW ){
        digitalWrite(pinLed, LOW);
        buttonPressed = false;
        //Serial.println("\ndelaying...");
        //delay(2000);
      }
    }
  
  /*
  if (val == 11) {

    // ENTRA AL SEGUENT NIVELL
    level = min(level+1,1);

  } else if (val == 10) {

    // SURT AL NIVELL ANTERIOR
    level = max(level-1,0);

  }else{
  */

    switch(level){

      case 0:
        menuSelection = ( menuSelection + 4 + val ) % 4;
        val = 0;
   
        if(previousMenu != menuSelection){
          //Serial.print("Menu: ");
          //Serial.println(menuSelection);
          previousMenu = menuSelection;
          
          switch(menuSelection) {

            case 0:
              Serial.print("1. Number of photos: ");
              Serial.println(value[menuSelection]);
              break;
              
            case 1:
              Serial.print("2. Interval time: ");
              Serial.println(value[menuSelection]);
              break;
              
            case 2:
              Serial.print("3. Focus adjust: ");
              Serial.println(value[menuSelection]);
              break;
              
            case 3:
              Serial.print("4. Exposure adjust: ");
              Serial.println(value[menuSelection]);
              break;
              
            default: 
              Serial.println("Undefined menu!");
            break;
          }
        
        }
        break;
        
      case 1:
        
        if( val != 0 ){

          value[menuSelection] += val;
          val = 0;
          Serial.print(menuSelection+1);
          Serial.print(". [");
          for(int i = 0; i < 3; i++){
            Serial.print(value[i]);
            Serial.print(",");
          }
          Serial.print(value[3]);
          Serial.println("]");
        }
        break;
        
      }

}
