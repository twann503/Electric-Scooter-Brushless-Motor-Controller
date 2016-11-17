#define OFF 0


boolean forward = true;
int Speed = 170;
int isReady = 0;

void setup() {
  Serial.begin(9600);
  pinMode(22, INPUT_PULLUP); //Yellow
  pinMode(24, INPUT_PULLUP); //Green
  pinMode(26, INPUT_PULLUP); //White
  
  pinMode(7, OUTPUT); //MOSI  Rp
  pinMode(6, OUTPUT); //5  Bp
  pinMode(5, OUTPUT); //4  Yn
  pinMode(4, OUTPUT); //3  Rn
  pinMode(3, OUTPUT); //2  Bn
  pinMode(2, OUTPUT); //1  Yp

  turnOff();
}

void loop() {
  int value1 = digitalRead(22);
  int value2 = digitalRead(24);
  int value3 = digitalRead(26);
  int values[3] = {value1, value2, value3};

  String CoilSelect = String(value1) + String(value2) + String(value3);
  Serial.println(CoilSelect);
  //turnOff();
  if(isReady > 0){
    if (CoilSelect == "001")         //s1
    {     analogWrite(7, OFF);  
          analogWrite(2, OFF);
          analogWrite(5,OFF);
          analogWrite(3,OFF);
          analogWrite(4, Speed);
          analogWrite(6,Speed);
          Serial.println(isReady);
          delay(2);
    }
    else if (CoilSelect == "011")
    {
          analogWrite(7, OFF);  
          analogWrite(3,OFF);
          analogWrite(4,OFF);
          analogWrite(2,OFF);
          analogWrite(5,Speed);
          analogWrite(6, Speed);
          Serial.println(isReady);
          delay(2);
    }
    else if (CoilSelect == "010")
    {
          analogWrite(4, OFF);  
          analogWrite(6,OFF);
          analogWrite(3,OFF);
          analogWrite(2,OFF);
          analogWrite(5, Speed);
          analogWrite(7,Speed);
          Serial.println(isReady);
          delay(2);
    }
    else if (CoilSelect == "110")
    {
          analogWrite(4, OFF);  
          analogWrite(6, OFF);
          analogWrite(2,OFF);
          analogWrite(5,OFF);
          analogWrite(3,Speed);
          analogWrite(7,Speed);
          Serial.println(isReady);
          delay(2);
    }
    else if (CoilSelect == "100")
    { 
          analogWrite(6, OFF);
          analogWrite(5,OFF);
          analogWrite(4,OFF);
          analogWrite(7,OFF);
          analogWrite(3, Speed); 
          analogWrite(2,Speed);
          Serial.println(isReady);
          delay(2);
    }
    else if (CoilSelect == "101")
    { 
          analogWrite(6, OFF);
          analogWrite(5,OFF);
          analogWrite(3,OFF);
          analogWrite(7,OFF);
          analogWrite(4,Speed);
          analogWrite(2, Speed); 
          Serial.println(isReady);
          delay(2);
    }
    --isReady;
  }
  else{
      turnOff();
  }
  //delay()
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
      isReady = 1000;
      Serial.println("Going ");
      return;
    }
  }
  return;
}

