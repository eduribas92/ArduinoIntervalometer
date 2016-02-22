/*
Per cada nou menu, modificar:
- numOptions
- levelOption
- valueOption del menu
- afegir cases a menu i a print
*/

//------------LCD-----------------

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const int pinBackLight = 10;
int columns = 20, rows = 4;

//------------BUTTONS-------------

const int pinButtonInc = 12, pinButtonDec = 13, pinButtonIntro = 11;
int stateButtonInc = 0, stateButtonDec = 0, stateButtonIntro = 0;

boolean buttonPressed = false;
unsigned long timeButtonPressed = 0;
const int precision = 200; // (ms)

//------------MENU----------------

int levelInc = 0, value = 0, level = 0, selectedOption = 0, numOptions = 7;
int levelOption[] = {2,2,2,2,1,2,2}; // number of levels for each menu

//------------PARAMETERS----------

// valueOption = [totalValueOption, valueOptionLevel1, valueOptionLevel2,...]
int valueOption_NumPhotos[] = {3,3};
long valueOption_IntTime[] = {1,1}; // (ms)
long valueOption_Focus[] = {1,1}; // (ms)
long valueOption_Expo[] = {1,1}; // (ms)
int valueOption_BackLight[] = {5,5};
int FPS = 24;
int valueOption_Duration = valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0];
int valueOption_Shooting[] = {0,0};
// FICAR-HO TOT EN UN VECTOR DE VECTORS? valueOption = [valueOption1, valueOption2, ...] 
// --> SWITCH MOLT FACIL, NOMES CAL SUMAR VALUE AMB COORDENADES valueOption[selectedOption][levelOption]

//------------SHOOTING------------

int optoFocus = 8, optoShutter = 9;
int shootingMenu = 0;
bool shooting = false;
int startingDelay = 5; // (s)
int counterPhotos = 0;
int previousState = -1, state = 0;// 0: waiting, 1: focusing, 2: shooting, 4: finished
unsigned long stateTime; // en quin moment ho assignare?? 
// si faig un time-lapse amb un gran interval time, trigara molt a començar!
// fer stateTime = 10s? o startingDelay x2, o simliar, o directament = 0 i amb el startingDelay ja ho controlo
unsigned long timeInitial, timePrintInfo;

//--------------------------------

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(columns, rows);
  pinMode(pinBackLight, OUTPUT);
  analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 10, 0, 255) );
  printMenuLCD(selectedOption);
     
  // initialize the pushbutton pin as an input:
  pinMode(pinButtonInc, INPUT);
  pinMode(pinButtonDec, INPUT);
  pinMode(pinButtonIntro, INPUT); 

  // initialize output digital pins
  pinMode(optoFocus, OUTPUT);
  pinMode(optoShutter, OUTPUT);
  digitalWrite(optoFocus, LOW);
  digitalWrite(optoShutter, LOW);
  
}

void loop() {

  // read the state of the pushbutton value if has passed enough time:
  if( ( millis() - timeButtonPressed ) > precision ){
    timeButtonPressed = millis();
    stateButtonInc = digitalRead(pinButtonInc);
    stateButtonDec = digitalRead(pinButtonDec);
    stateButtonIntro = digitalRead(pinButtonIntro);

    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if( stateButtonInc == HIGH ) value = 1;

    if( stateButtonDec == HIGH ) value = -1;

    if( buttonPressed == false && stateButtonIntro == HIGH ){
      levelInc = 1;
      buttonPressed = true; 
    } 

    if( stateButtonIntro == LOW ) buttonPressed = false;
  }

  if( !shooting ){ // if not shooting

    // segons el "menu Seleccionat", level pot prendre mes o menys valors
    // if 'level' has changed, update it
    if( levelInc != 0 ){
      level = (level + levelInc) % levelOption[selectedOption];
      levelInc = 0;
      lcd.setCursor(0,0);
      if(level == 0) lcd.print("*");
      else lcd.print(" ");
    }
  
    if( value != 0 ){
      
      if(level == 0){
        selectedOption = ( selectedOption + numOptions + value ) % numOptions;
        value = 0; 
        printMenuLCD(selectedOption);
        
      }else{
        // segons menu i level, canvia els paràmetres adequats
        switch(selectedOption) {
          case 0: // 1. Number of photos:
            valueOption_NumPhotos[level] += value;
            value = 0;
            valueOption_NumPhotos[0] = valueOption_NumPhotos[1];
            valueOption_Duration = valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0];
            break;
          case 1: // 2. Interval time:
            valueOption_IntTime[level] += value;
            value = 0;
            valueOption_IntTime[0] = valueOption_IntTime[1];
            valueOption_Duration = valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0];
            break;
          case 2: // 3. Focus adjust:
            valueOption_Focus[level] += value;
            value = 0;
            valueOption_Focus[0] = valueOption_Focus[1];
            valueOption_Duration = valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0];
            break;
          case 3: // 4. Exposure adjust:
            valueOption_Expo[level] += value;
            value = 0;
            valueOption_Expo[0] = valueOption_Expo[1];
            valueOption_Duration = valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0];
            break;
          case 5: // 6. Back. light
            valueOption_BackLight[1] = max( min( 10, valueOption_BackLight[1] + value ), 0 );
            value = 0;
            valueOption_BackLight[0] = valueOption_BackLight[1];
            analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 10, 0, 255) );
            break;
          case 6: // 7. Start shooting:
            valueOption_Shooting[level] = ( valueOption_Shooting[level] + 2 + value ) % 2;
            value = 0;
            valueOption_Shooting[0] = valueOption_Shooting[1];
            // PERMETRE AJUSTAR AQUEST DELAY!!
            break;
          default: 
            break;
        }
        printMenuLCD(selectedOption);
      }  
         
    }

  }

  if( !shooting && level == 0 && valueOption_Shooting[0] != 0 ){
    lcd.clear();
    shooting = true;
    state = 0;
    stateTime = 0;
    previousState = -1;
    valueOption_Shooting[0] = valueOption_Shooting[1] = 0;
    int counterDelay = startingDelay;
    while(counterDelay){
      lcd.clear();
      lcd.print(counterDelay);
      // PRINT ON SCREEN!
      counterDelay--;
      // turn on LED
      delay(500);
      // turn off LED
      delay(500);
    }
    timePrintInfo = 0;
    timeInitial = millis();
  }

  if( shooting ){

    if(state == 0 && millis() > stateTime){ 
      // start focusing
      stateTime = millis() + valueOption_Focus[0]*1000;
      state = 1;
      lcd.clear();
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      lcd.print(" focusing...");
      digitalWrite(optoFocus, HIGH);
    
    }

    if(state == 1 && millis() > stateTime){ 
      // start shooting
      stateTime = millis() + valueOption_Expo[0]*1000;
      state = 2;
      lcd.clear();
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      lcd.print(" shooting...");
      digitalWrite(optoShutter, HIGH);
 
    }

    if(state == 2 && millis() > stateTime){
      // stops focusing and shooting, starts waiting
      digitalWrite(optoFocus, LOW);
      digitalWrite(optoShutter, LOW);
      counterPhotos++;
      lcd.clear();
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      
      if(counterPhotos != valueOption_NumPhotos[0]){
        state = 0;
        stateTime = millis() + valueOption_IntTime[0]*1000;
        lcd.print(" waiting...");
      }
      
      if(counterPhotos == valueOption_NumPhotos[0]){
        state = 4;
        lcd.setCursor(0,2);
        lcd.print("Shooting completed!");
        lcd.setCursor(0,3);
        lcd.print("(Press any button)");
        counterPhotos = 0;
        valueOption_BackLight[0] = valueOption_BackLight[1] = 5;
        analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 10, 0, 255) );   
      }
    }

    
    if(state != 4){
      // PRINT DYNAMIC INFO
      if( (millis() - timePrintInfo ) > 1000 ){
        timePrintInfo = millis();
        lcd.setCursor(0,1);
        lcd.print("Remaining: ");
        lcd.print( max(0, valueOption_Duration - (timePrintInfo - timeInitial)/1000 ) );
      }
      // PRINT STATIC INFO
      if( state != previousState ){
        previousState = state;
        lcd.setCursor(0,3);
        lcd.print("Light: ");
        lcd.print(valueOption_BackLight[0]);
      }
    }
    
    if(state == 4){
      if(value != 0 || levelInc != 0){
        state = 0;
        value = levelInc = 0;
        shooting = false;
        level = 0;
        selectedOption = 0,
        printMenuLCD(selectedOption);
      }
    }

    
    
    if(value != 0){
      valueOption_BackLight[1] = max( min( 10, valueOption_BackLight[1] + value ), 0 );
      value = 0;
      valueOption_BackLight[0] = valueOption_BackLight[1];
      analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 10, 0, 255) );
      lcd.setCursor(0,3);
      lcd.print("Light: ");
      lcd.print(valueOption_BackLight[0]);
    }

    // BOTTONS UP I DOWN CONTROLEN LLUM DE BACKGROUND
    // MANTENIR ENTER PREMUT (3 segons) PERMET ACCEDIR A MENU: PAUSE/INIT, RESTART, RETURN
    // QUE DEMANI PER CONFIRMAR

    // AFEGIR MÉS INFO A LA PANTALLA
    
  }
}


void MenuLCD(int selectedOption){
  switch(selectedOption) {
    case 0:
      lcd.print("1 Photos: ");
      lcd.print(valueOption_NumPhotos[0]);
      break;              
    case 1:
      lcd.print("2 Interval: ");
      lcd.print(valueOption_IntTime[0]);
      break;              
    case 2:
      lcd.print("3 Focus: ");
      lcd.print(valueOption_Focus[0]);
      break;              
    case 3:
      lcd.print("4 Exposure: ");
      lcd.print(valueOption_Expo[0]);
      break;
    case 4:
      lcd.print("5 Duration: ");
      lcd.print(valueOption_Duration);
      break;
    case 5:
      lcd.print("6 Light: ");
      lcd.print(valueOption_BackLight[0]);
      break;
    case 6:
      lcd.print("* START: ");
      if(valueOption_Shooting[0] == 0) lcd.print("No");
      if(valueOption_Shooting[0] == 1) lcd.print("Yes");
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
  if(level == 0) lcd.print("*");
  else lcd.print(" ");
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
