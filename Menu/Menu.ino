int entry = 0;
int val = 1;

String incomingString;
int menuSelection
;
int previousMenu = -1;

int value[] = {0,0,0,0,0};

void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("initializing...");
}

void loop() {

  if(Serial.available() > 0){
    char ch = Serial.read();
    
    if(ch != 10){
      incomingString += ch;
    }else{
      val = incomingString.toInt();
      
      incomingString = "";
      
    }
  }
  
  switch(entry){
    case 0:
      if(val == 11){
        // ENTRA AL SEGUENT NIVELL
        entry = min(entry+1,1);
        break;
      }
      
      if(val == 10){
        // SURT AL NIVELL ANTERIOR
        entry = max(entry-1,0);
        break;
      }
      
      menuSelection = val;
 
      if(previousMenu != menuSelection){
        //Serial.print("Menu: ");
        //Serial.println(menuSelection);
        previousMenu = menuSelection;
        
        switch(menuSelection) {
          
          case 1:
            Serial.print("1. Number of photos: ");
            Serial.println(value[menuSelection]);
            break;
            
          case 2:
            Serial.print("2. Interval time: ");
            Serial.println(value[menuSelection]);
            break;
            
          case 3:
            Serial.print("3. Focus adjust: ");
            Serial.println(value[menuSelection]);
            break;
            
          case 4:
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
    
      if(val == 11){
        // ENTRA AL SEGUENT NIVELL
        entry = min(entry+1,1);
        break;
      }
      
      if(val == 10){
        // SURT AL NIVELL ANTERIOR
        entry = max(entry-1,0);
        break;
      }
      
      if(value[menuSelection] != val){
        value[menuSelection] = val;
        Serial.print(menuSelection);
        Serial.print(". [");
        for(int i = 0; i < 4; i++){
          Serial.print(value[i]);
          Serial.print(",");
        }
        Serial.print(value[4]);
        Serial.println("]");
      }
      break;
        
  }
}
