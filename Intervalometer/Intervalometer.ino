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

unsigned long timeButtonPressed = 0;
const int precision = 200; // (ms)

//------------MENU----------------

int levelInc = 0, value = 0, level = 0, selectedOption = 0, numOptions = 8;
int levelOption[][8] = {                          // 1rst pos: number of levels      rest: position of each level
                        {5,9,10,11,12},        // NumPhotos
                        {8,7,8,10,11,13,14,15}, // IntTime
                        {5,8,10,11,12},           // Focus
                        {8,7,8,10,11,13,14,15}, // Expo
                        {1},                      // Duration
                        {2,8},                    // BackLight
                        {2,8},                    // Shooting
                        {1}};                     // --------   
             
//------------PARAMETERS----------

// valueOption = [totalValueOption, valueOptionLevel1, valueOptionLevel2,...]
long valueOption_NumPhotos[] = {0,0,0,0,0}; // 0000 - 9999
long valueOption_NumPhotos_Limits[] = {0,10,10,10,10};
long valueOption_IntTime[] = {0,0,0,0,0,0,0,0}; // (ms) ..m..,...s
long valueOption_IntTime_Limits[] = {0,10,10,6,10,10,10,10};
long valueOption_Focus[] = {0,0,0,0,0}; // (ms) .,...s
long valueOption_Focus_Limits[] = {0,10,10,10,10};
long valueOption_Expo[] = {0,0,0,0,0,0,0,0}; // (ms) ..m..,...s
long valueOption_Expo_Limits[] = {0,10,10,6,10,10,10,10};
int FPS = 24;
unsigned long valueOption_Duration = (valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0])/1000; // s
int valueOption_BackLight[] = {5,5};
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
unsigned long stateTime, timeInitial, timePrintInfo;

//--------------------------------

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(columns, rows);
  pinMode(pinBackLight, OUTPUT);
  analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 9, 0, 255) );
  printMenuLCD(selectedOption, level);
     
  // initialize the pushbutton pins as an inputs:
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

  // 0. READ BUTTONS (if has passed enough time):
  if( ( millis() - timeButtonPressed ) > precision ){
    timeButtonPressed = millis();
    stateButtonInc = digitalRead(pinButtonInc);
    stateButtonDec = digitalRead(pinButtonDec);
    stateButtonIntro = digitalRead(pinButtonIntro);

    if( stateButtonInc == HIGH ) value = 1;
    if( stateButtonDec == HIGH ) value = -1;
    if( stateButtonIntro == HIGH ) levelInc = 1;
  }

  // 1. IF NOT SHOOTING
  if( !shooting ){ 

    // UPDATE LEVEL
    if( levelInc != 0 ){
      level = (level + levelInc) % levelOption[selectedOption][0];
      levelInc = 0;
      printMenuLCD(selectedOption, level);
    }
    
    // UPDATE VALUE...
    if( value != 0 ){
      
      // ... of selecion
      if(level == 0){
        selectedOption = ( selectedOption + numOptions + value ) % numOptions;
        value = 0; 
        printMenuLCD(selectedOption, level);
      
      // ... of option selected 
      }else{
        // segons menu i level, canvia els paràmetres adequats
        switch(selectedOption) {
          
          case 0: // 1. Number of photos:
            valueOption_NumPhotos[level] = ( valueOption_NumPhotos[level] + value + valueOption_NumPhotos_Limits[level] ) % valueOption_NumPhotos_Limits[level] ;
            value = 0;
            valueOption_NumPhotos[0] = valueOption_NumPhotos[1]*1000 + valueOption_NumPhotos[2]*100 + valueOption_NumPhotos[3]*10 + valueOption_NumPhotos[4];
            break;
          
          case 1: // 2. Interval time:
            valueOption_IntTime[level] = ( valueOption_IntTime[level] + value + valueOption_IntTime_Limits[level] ) % valueOption_IntTime_Limits[level] ;
            value = 0;
            valueOption_IntTime[0] = valueOption_IntTime[1]*10*60*1000 + valueOption_IntTime[2]*60*1000 + valueOption_IntTime[3]*10*1000 + valueOption_IntTime[4]*1000 + valueOption_IntTime[5]*100 + valueOption_IntTime[6]*10 + valueOption_IntTime[7];
            break;
          
          case 2: // 3. Focus adjust:
            valueOption_Focus[level] = ( valueOption_Focus[level] + value + valueOption_Focus_Limits[level] ) % valueOption_Focus_Limits[level] ;
            value = 0;
            valueOption_Focus[0] = valueOption_Focus[1]*1000 + valueOption_Focus[2]*100 + valueOption_Focus[3]*10 + valueOption_Focus[4];
            break;
          
          case 3: // 4. Exposure adjust:
            valueOption_Expo[level] = ( valueOption_Expo[level] + value + valueOption_Expo_Limits[level] ) % valueOption_Expo_Limits[level] ;
            value = 0;
            valueOption_Expo[0] =  valueOption_Expo[1]*10*60*1000 +  valueOption_Expo[2]*60*1000 +  valueOption_Expo[3]*10*1000 +  valueOption_Expo[4]*1000 +  valueOption_Expo[5]*100 +  valueOption_Expo[6]*10 +  valueOption_Expo[7];            break;
          
          case 5: // 6. Back. light
            valueOption_BackLight[1] = max( min( 9, valueOption_BackLight[1] + value ), 0 );
            value = 0;
            valueOption_BackLight[0] = valueOption_BackLight[1];
            analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 9, 0, 255) );
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
        valueOption_Duration = (valueOption_NumPhotos[0]*(valueOption_IntTime[0] + valueOption_Focus[0] + valueOption_Expo[0]) - valueOption_IntTime[0])/1000;
        printMenuLCD(selectedOption, level);
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
      counterDelay--;
      delay(1000);
    }
    timePrintInfo = 0;
    timeInitial = millis();
  }

  if( shooting ){
    
    if(counterPhotos == valueOption_NumPhotos[0]){
      state = 4;
      counterPhotos = 0;
      valueOption_BackLight[0] = valueOption_BackLight[1] = 5;
      analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 9, 0, 255) );
      
      lcd.setCursor(0,2);
      lcd.print("Shooting completed!");
      lcd.setCursor(0,3);
      lcd.print("(Press any button)");
         
    }

    if(state == 0 && millis() > stateTime){ 
      // start focusing
      stateTime = millis() + valueOption_Focus[0];
      state = 1;
      digitalWrite(optoFocus, HIGH);
      
      lcd.setCursor(0,0);
      lcd.print("                    ");
      lcd.setCursor(0,0);
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      lcd.print(" focusing...");
    }

    if(state == 1 && millis() > stateTime){ 
      // start shooting
      stateTime = millis() + valueOption_Expo[0];
      state = 2;
      digitalWrite(optoShutter, HIGH);
      
      lcd.setCursor(0,0);
      lcd.print("                    ");
      lcd.setCursor(0,0);
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      lcd.print(" shooting...");
    }

    if(state == 2 && millis() > stateTime){
      // stops focusing and shooting, starts waiting
      stateTime = millis() + valueOption_IntTime[0];
      state = 0;
      digitalWrite(optoFocus, LOW);
      digitalWrite(optoShutter, LOW);
      counterPhotos++;
      
      lcd.setCursor(0,0);
      lcd.print("                    ");
      lcd.setCursor(0,0);
      lcd.print(counterPhotos);
      lcd.print("/");
      lcd.print(valueOption_NumPhotos[0]);
      lcd.print(" waiting...");
    }

    
    if(state != 4){
      // PRINT DYNAMIC INFO
      if( (millis() - timePrintInfo ) > 1000 ){
        timePrintInfo = millis();
        lcd.setCursor(0,1);
        lcd.print("Remaining: ");
        lcd.print( max(0, valueOption_Duration - (timePrintInfo - timeInitial)/1000 ) );
        lcd.print("s");
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
        printMenuLCD(selectedOption, level);
      }
    }

    
    if(value != 0){
      valueOption_BackLight[1] = max( min( 9, valueOption_BackLight[1] + value ), 0 );
      value = 0;
      valueOption_BackLight[0] = valueOption_BackLight[1];
      analogWrite(pinBackLight, map(valueOption_BackLight[0], 0, 9, 0, 255) );
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
      lcd.print(" Photos: ");
      lcd.print(valueOption_NumPhotos[1]);
      lcd.print(valueOption_NumPhotos[2]);
      lcd.print(valueOption_NumPhotos[3]);
      lcd.print(valueOption_NumPhotos[4]);
      break;              
    case 1:
      lcd.print(" Int.: ");
      lcd.print(valueOption_IntTime[1]);
      lcd.print(valueOption_IntTime[2]);
      lcd.print("m");
      lcd.print(valueOption_IntTime[3]);
      lcd.print(valueOption_IntTime[4]);
      lcd.print(",");
      lcd.print(valueOption_IntTime[5]);
      lcd.print(valueOption_IntTime[6]);
      lcd.print(valueOption_IntTime[7]);
      lcd.print("s");
      break;              
    case 2:
      lcd.print(" Focus: ");
      lcd.print(valueOption_Focus[1]);
      lcd.print(",");
      lcd.print(valueOption_Focus[2]);
      lcd.print(valueOption_Focus[3]);
      lcd.print(valueOption_Focus[4]);
      lcd.print("s");
      break;              
    case 3:
      lcd.print(" Exp.: ");
      lcd.print(valueOption_Expo[1]);
      lcd.print(valueOption_Expo[2]);
      lcd.print("m");
      lcd.print(valueOption_Expo[3]);
      lcd.print(valueOption_Expo[4]);
      lcd.print(",");
      lcd.print(valueOption_Expo[5]);
      lcd.print(valueOption_Expo[6]);
      lcd.print(valueOption_Expo[7]);
      lcd.print("s");
      break;
    case 4:
      lcd.print(" Durat.: ");
      lcd.print(valueOption_Duration);
      lcd.print("s");
      break;
    case 5:
      lcd.print(" Light: ");
      lcd.print(valueOption_BackLight[0]);
      break;
    case 6:
      lcd.print(" START: ");
      if(valueOption_Shooting[0] == 0) lcd.print("No");
      if(valueOption_Shooting[0] == 1) lcd.print("Yes");
      break;
    case 7:
      lcd.print(" ------------------");
      break;
    default: 
      lcd.print(" Undefined menu!");
      break;
  }
  return;
}


void printMenuLCD(int menuToPrint, int level){
  //boolean line = true;
  lcd.clear();

  if(level == 0){
    for(int i = 0; i < 4; i++){
      lcd.setCursor(0,i);
      MenuLCD( (menuToPrint + i) % numOptions ); 
    }
    lcd.setCursor(0,0);
    lcd.print("*");
        
  }else{
    lcd.setCursor(0,0);
    MenuLCD( menuToPrint );

    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(levelOption[menuToPrint][level],1);
    lcd.print("*");

    lcd.setCursor(0,2);
    MenuLCD( (menuToPrint + 2) % numOptions );

    lcd.setCursor(0,3);
    MenuLCD( (menuToPrint + 3) % numOptions );    
  }
  
  /*for(int i = 1; i < 4; i++){
    if(menuToPrint == numOptions-1 && line){
      lcd.setCursor(0,i);
      line = false;
      lcd.print(" ------------------ ");
    }else{
    lcd.setCursor(1,i);
    menuToPrint = (menuToPrint + 1) % numOptions;
    MenuLCD(menuToPrint); 
    }  
  }*/

  return;
}
