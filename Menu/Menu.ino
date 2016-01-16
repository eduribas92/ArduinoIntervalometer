long loops = 0;
int seconds = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  loops++;
  if(millis()/1000 > seconds){
    seconds = millis()/1000;
    Serial.println(loops);
  }
}
