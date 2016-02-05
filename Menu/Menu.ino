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
int levelInc = 0;

String incomingString;
int menuSelection = 0;

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
  printMenu(menuSelection,value[menuSelection]);
}

void loop() {

  // read the state of the pushbutton value if has passed enough time:
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
      levelInc = +1;
      /*Serial.print("Level: ");
      Serial.println(level);*/
      buttonPressed == true; 
    } 
    
    if( stateButton1 == LOW && stateButton2 == LOW && stateButton3 == LOW ){
      digitalWrite(pinLed, LOW);
      buttonPressed = false;
    }
  }

  //segons el "menu Seleccionat", level pot prendre mes o menys valors, i els paràmetres que es modifiquen son diferents
  if( levelInc !=0 || val != 0 ){

    switch(menuSelection) {

      case 0: // 1. Number of photos:
        
        // if 'level' has changed, update it
        if( levelInc != 0 ){
          level = (level + levelInc) % 2;
          levelInc = 0;
        }

        // if 'val' has changed, update depending on 'level'
        if( val != 0 ){
          switch(level){
            case 0:
              menuSelection = ( menuSelection + 4 + val ) % 4;
              val = 0; 
              printMenu(menuSelection,value[menuSelection]);  
              break;
              
            case 1:
              value[menuSelection] += val;
              val = 0;
              Serial.print("\t");
              Serial.println(value[menuSelection]);
              break;
          }
        }

        break;
              
      case 1: // 2. Interval time:

        // if 'level' has changed, update it
        if( levelInc != 0 ){
          level = (level + levelInc) % 2;
          levelInc = 0;
        }

        // if 'val' has changed, update depending on 'level'
        if( val != 0 ){
          switch(level){
            case 0:
              menuSelection = ( menuSelection + 4 + val ) % 4;
              val = 0; 
              printMenu(menuSelection,value[menuSelection]);  
              break;
              
            case 1:
              value[menuSelection] += val;
              val = 0;
              Serial.print("\t");
              Serial.println(value[menuSelection]);
              break;
          }
        }

        break;
              
      case 2: // 3. Focus adjust:

        // if 'level' has changed, update it
        if( levelInc != 0 ){
          level = (level + levelInc) % 2;
          levelInc = 0;
        }

        // if 'val' has changed, update depending on 'level'
        if( val != 0 ){
          switch(level){
            case 0:
              menuSelection = ( menuSelection + 4 + val ) % 4;
              val = 0; 
              printMenu(menuSelection,value[menuSelection]);  
              break;
              
            case 1:
              value[menuSelection] += val;
              val = 0;
              Serial.print("\t");
              Serial.println(value[menuSelection]);
              break;
          }
        }

        break;
              
      case 3: // 4. Exposure adjust:
        
        // if 'level' has changed, update it
        if( levelInc != 0 ){
          level = (level + levelInc) % 2;
          levelInc = 0;
        }

        // if 'val' has changed, update depending on 'level'
        if( val != 0 ){
          switch(level){
            case 0:
              menuSelection = ( menuSelection + 4 + val ) % 4;
              val = 0; 
              printMenu(menuSelection,value[menuSelection]);  
              break;
              
            case 1:
              value[menuSelection] += val;
              val = 0;
              Serial.print("\t");
              Serial.println(value[menuSelection]);
              break;
          }
        }
        
        break;
              
      default: 
        Serial.println("Undefined menu!");
        break;

    }
  }
}


void printMenu( int menuSelection, int value ){

  Serial.print("\n");

  switch(menuSelection) {

    case 0:
      Serial.print("1. Number of photos: ");
      break;
              
    case 1:
      Serial.print("2. Interval time: ");
      break;
              
    case 2:
      Serial.print("3. Focus adjust: ");
      break;
              
    case 3:
      Serial.print("4. Exposure adjust: ");
      break;
  }
  
  Serial.println(value);

  return;
}
