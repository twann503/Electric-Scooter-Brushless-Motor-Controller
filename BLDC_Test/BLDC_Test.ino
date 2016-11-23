#define OFF 0
#define SEN A2
#include "TimerOne.h"



volatile int Speed = 170;
int isReady = 0;
int states = 7;
int count = 0;
volatile int noSpeedCount = 0;
double Time = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;
int prevState = 7;
volatile int rpm = 0.0;


void setup() {
  Serial.begin(115200);
  pinMode(22, INPUT_PULLUP); //Yellow
  pinMode(24, INPUT_PULLUP); //Green
  pinMode(26, INPUT_PULLUP); //White
  
  pinMode(7, OUTPUT); //MOSI  Rp
  pinMode(6, OUTPUT); //5  Bp
  pinMode(5, OUTPUT); //4  Yn
  pinMode(4, OUTPUT); //3  Rn
  pinMode(3, OUTPUT); //2  Bn
  pinMode(2, OUTPUT); //1  Yp

  pinMode(A2, INPUT);

  Timer1.initialize(50000);
  Timer1.attachInterrupt(defSpeed);
  startTime = millis();
  turnOff();
}

void defSpeed(){
  Speed = map(analogRead(SEN), 175, 869, 0, 200);
  if(Speed < 0)
    Speed = 0;

  ++noSpeedCount;
  if(noSpeedCount >= 5){
    rpm = 0;
    noSpeedCount = 0;
  }
    
}

void loop() {
  states = ((digitalRead(22) << 2) + (digitalRead(24) << 1) + digitalRead(26));
  //Serial.print(states);Serial.print('\t');Serial.print(Speed);Serial.print('\t');Serial.print(rpm);Serial.print('\t');Serial.println(isReady);

  if(prevState != states){
     ++count;
     prevState = states;
  }

  if(count >= 45){
    endTime = millis();
    Time = endTime - startTime;
    Time = Time / 1000;
    rpm = 60/Time;
    count = 0;
    startTime = millis();
    noSpeedCount = 0;
  }

  if(isReady > 0){
   switch (states) {
    
   case 1:
     analogWrite(7, OFF);  
     analogWrite(2, OFF);
     analogWrite(5, OFF);
     analogWrite(3, OFF);
     analogWrite(4, Speed);
     analogWrite(6, Speed);
     break;


   case 2:
     analogWrite(4, OFF);  
     analogWrite(6, OFF);
     analogWrite(3, OFF);
     analogWrite(2, OFF);
     analogWrite(5, Speed);
     analogWrite(7, Speed);
     break;


   case 3:
     analogWrite(7, OFF);  
     analogWrite(3, OFF);
     analogWrite(4, OFF); 
     analogWrite(2, OFF);
     analogWrite(5, Speed);
     analogWrite(6, Speed);
     break;

   case 4:
     analogWrite(6, OFF);
     analogWrite(5, OFF);
     analogWrite(4, OFF);
     analogWrite(7, OFF);
     analogWrite(3, Speed);
     analogWrite(2, Speed);
     break;

   case 5:
     analogWrite(6, OFF);
     analogWrite(5, OFF);
     analogWrite(3, OFF);
     analogWrite(7, OFF);
     analogWrite(4, Speed);
     analogWrite(2, Speed); 
     break;

   case 6:
     analogWrite(4, OFF);  
     analogWrite(6, OFF);
     analogWrite(2, OFF);
     analogWrite(5, OFF);
     analogWrite(3, Speed);
     analogWrite(7, Speed);
     break;


   case 7: 
     turnOff();
     break;
    }
   //--isReady;
  }
  else
    turnOff();
}

void turnOff(){
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void serialEvent(){
  while(Serial.available()) {
    char inChar = (char)Serial.read();  
    if(inChar == ' '){
      isReady = 30000;
      Serial.println("Going ");
      return;
    }
  }
  return;
}

