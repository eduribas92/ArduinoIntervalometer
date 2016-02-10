//------------LCD-----------------

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int columns = 20;
int rows = 4;


//------------BUTTONS-------------

const int pinButton1 = 8, pinButton2 = 9, pinButton3 = 10;
int stateButton1 = 0, stateButton2 = 0, stateButton3 = 0;

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

int levelInc = 0, value = 0, level = 0, selectedOption = 0, numOptions = 4;
int levelOption[] = {2,2,2,2}; // number of levels for each menu

//------------PARAMETERS----------

// valueOption = [totalValueOption, valueOptionLevel1, valueOptionLevel2,...]
int valueOption_NumPhotos[] = {0,0};
int valueOption_IntTime[] = {0,0};
int valueOption_Focus[] = {0,0};
int valueOption_Expo[] = {0,0};
// FICAR-HO TOT EN UN VECTOR DE VECTORS? valueOption = [valueOption1, valueOption2, ...] 
// --> SWITCH MOLT FACIL, NOMES CAL SUMAR VALUE AMB COORDENADES valueOption[selectedOption][levelOption]

//--------------------------------

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(columns, rows);
  
  // initialize the LED pin as an output:
  //pinMode(pinLed, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT); 

  Serial.begin(9600);
  Serial.println("\ninitializing...");
  printMenuSerial(selectedOption);
  printMenuLCD(selectedOption);
  //printMenu(selectedOption,value[selectedOption]);
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
      //digitalWrite(pinLed, HIGH);
      value = 1;
      buttonPressed == true;  // QUÈ PASSA SI HO ESCRIC CORRECTAMENT???? DEIXARA D'AUMENTAR EL VALOR GRADUALMENT??? 
    } 
    // PUC DEIXAR-HO DE LA FORMA SEGÜENT?
    // if( buttonPressed == false && stateButton1 == HIGH ) val = 1;

    if( buttonPressed == false && stateButton2 == HIGH ) {     
      //digitalWrite(pinLed, HIGH);
      value = -1;
      buttonPressed == true;  
    } 

    if( buttonPressed == false && stateButton3 == HIGH ) {      
      //digitalWrite(pinLed, HIGH);
      levelInc = +1;
      buttonPressed == true; 
    } 

    // --> pregunta: ENTRA A AQUEST IF???
    if( stateButton1 == LOW && stateButton2 == LOW && stateButton3 == LOW ){
      //digitalWrite(pinLed, LOW);
      buttonPressed = false;
    }
  }

  // segons el "menu Seleccionat", level pot prendre mes o menys valors
  // if 'level' has changed, update it
  if( levelInc != 0 ){
    level = (level + levelInc) % levelOption[selectedOption];
    levelInc = 0;
  }
  
  if( value != 0 ){
    if(level == 0){
      selectedOption = ( selectedOption + numOptions + value ) % numOptions;
      value = 0; 
      printMenuSerial(selectedOption);
      printMenuLCD(selectedOption);
      //printMenu(selectedOption,value[selectedOption]);
    }else{
      // segons menu i level, canvia els paràmetres adequats
      switch(selectedOption) {
        case 0: // 1. Number of photos:
          valueOption_NumPhotos[level] += value;
          value = 0;
          valueOption_NumPhotos[0] = valueOption_NumPhotos[1];
          Serial.print("\t");
          Serial.println(valueOption_NumPhotos[0]);
          break;
        case 1: // 2. Interval time:
          valueOption_IntTime[level] += value;
          value = 0;
          valueOption_IntTime[0] = valueOption_IntTime[1];
          Serial.print("\t");
          Serial.println(valueOption_IntTime[0]);
          break;
        case 2: // 3. Focus adjust:
          valueOption_Focus[level] += value;
          value = 0;
          valueOption_Focus[0] = valueOption_Focus[1];
          Serial.print("\t");
          Serial.println(valueOption_NumPhotos[0]);
          break;
        case 3: // 4. Exposure adjust:
          valueOption_Expo[level] += value;
          value = 0;
          valueOption_Expo[0] = valueOption_Expo[1];
          Serial.print("\t");
          Serial.println(valueOption_Expo[0]);
          break;
        default: 
          Serial.println("Undefined menu!");
          break;
      }
      printMenuLCD(selectedOption);
    }

    
  }
}

void printMenuSerial(int selectedOption){
//void printMenu( int selectedOption, int value ){
  Serial.print("\n");
  switch(selectedOption) {
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
    default: 
      Serial.println("Undefined menu!");
      break;
  }
  //Serial.println(value);
  return;
}

void MenuLCD(int selectedOption){
  switch(selectedOption) {
    case 0:
      lcd.print("1.Num. photos: ");
      lcd.print(valueOption_NumPhotos[0]);
      break;              
    case 1:
      lcd.print("2.Int. time: ");
      lcd.print(valueOption_IntTime[0]);
      break;              
    case 2:
      lcd.print("3.Focus adjust: ");
      lcd.print(valueOption_Focus[0]);
      break;              
    case 3:
      lcd.print("4.Expo. adjust: ");
      lcd.print(valueOption_Expo[0]);
      break;
    default: 
      lcd.print("Undefined menu!");
      break;
  }
  return;
}

void printMenuLCD(int selectedOption){
  boolean line = true;
  lcd.clear();
  lcd.print("*");
  MenuLCD(selectedOption);
  for(int i = 1; i < 4; i++){
    if(selectedOption == numOptions-1 && line){
      lcd.setCursor(0,i);
      line = false;
      lcd.print(" ------------------ ");
    }else{
      lcd.setCursor(1,i);
      selectedOption = (selectedOption + 1) % numOptions;
      MenuLCD(selectedOption);
    }
  }
  return;
}
