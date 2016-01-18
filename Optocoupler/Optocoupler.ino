
// printing string of characters and values
#include <stdio.h>
int l = 0;

// connections:
int optoFocus = 11;
int optoShutter = 12;

// shooting parameters:
int totalPhotos = 10; // number of photos
int counterPhotos = 0;
long intervalTime = 5000; // (ms)
long focusTime = 500; // (ms)
long expoTime = 1000; // (ms)
int FPS = 24;
int state = 0;// 0: waiting, 1: focusing, 2: shooting
unsigned long stateTime = intervalTime; // en quin moment ho assignare??
boolean start = true;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins 12 and 13 as outputs
  pinMode(optoFocus, OUTPUT);
  pinMode(optoShutter, OUTPUT);

  digitalWrite(optoFocus, LOW);
  digitalWrite(optoShutter, LOW);
  
  Serial.begin(9600);
  Serial.println("initializing...\n");

  char buffer [50];
  l=sprintf(buffer, "N: %d\n\n", counterPhotos);
  for(int i= 0; i<l; i++) 
    Serial.print(buffer[i]);
}

// the loop function runs over and over again forever
void loop(){
  
  if(start){

    if(state == 0 && millis() > stateTime){ 
      // start focusing
      stateTime = millis() + focusTime;
      state = 1;
      digitalWrite(optoFocus, HIGH);
      Serial.println("focusing...");
    }

    if(state == 1 && millis() > stateTime){ 
      // start shooting
      stateTime = millis() + expoTime;
      state = 2;
      digitalWrite(optoShutter, HIGH);
      Serial.println("shooting...");
    }

    if(state == 2 && millis() > stateTime){
      // stops focusing and shooting, starts waiting
      stateTime = millis() + intervalTime;
      state = 0;
      counterPhotos++;
      digitalWrite(optoFocus, LOW);
      digitalWrite(optoShutter, LOW);
      
      char buffer [50];
      l=sprintf(buffer, "N: %d\n\n", counterPhotos);
      for(int i= 0; i<l; i++) 
        Serial.print(buffer[i]);
      
      if(counterPhotos == totalPhotos)
        start = false;
    }

  }

}
