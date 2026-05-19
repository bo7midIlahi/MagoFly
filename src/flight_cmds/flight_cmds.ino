#include <Servo.h>
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

#define MOTOR1LED 15
#define MOTOR2LED 16
#define MOTOR3LED 17
#define MOTOR4LED 18

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00010";

struct userInputs {
  char altitude[4];
  char throttle[4];
  char light[3];
  char throttle_change_disable[3];
  char hand_setup[3];
  int emergency_landing;
  int engine_cut;
  int yaw;
  int pitch;
  int roll;
};
struct userInputs userIn;

void calibration(){
  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  ESC3.writeMicroseconds(2000);
  ESC4.writeMicroseconds(2000);

  delay(2000);

  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);

  delay(2000);
  Serial.println("DONE CALIBRATION");
  for(int iter=0; iter<2; iter++){
    ledBlink();
  }
}

void motorTest(){
  for (int i=0; i<=1000; i+=50) {
    ESC1.writeMicroseconds(1000 + i);
    ESC2.writeMicroseconds(1000 + i);
    ESC3.writeMicroseconds(1000 + i);
    ESC4.writeMicroseconds(1000 + i);

    Serial.print("INCREASING: ");
    Serial.println(1000+i);
  
    delay(500);
  }

  delay(2000);

  for (int i=1000; i>=0; i-=50) {
    ESC1.writeMicroseconds(1000 + i);
    ESC2.writeMicroseconds(1000 + i);
    ESC3.writeMicroseconds(1000 + i);
    ESC4.writeMicroseconds(1000 + i);

    Serial.print("DECREASING: ");
    Serial.println(1000+i);

    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  pinMode(2,OUTPUT);

  //attaching ESCs
  ESC1.attach(3,1000,2000);
  ESC2.attach(5,1000,2000);
  ESC3.attach(6,1000,2000);
  ESC4.attach(9,1000,2000);

  //setup pins
  pinMode(MOTOR1LED,OUTPUT);
  pinMode(MOTOR2LED,OUTPUT);
  pinMode(MOTOR3LED,OUTPUT);
  pinMode(MOTOR4LED,OUTPUT);

  calibration();
  //motorTest();
  delay(3000);
}

void ledBlink(){
  digitalWrite(MOTOR1LED,HIGH);
  digitalWrite(MOTOR2LED,HIGH);
  digitalWrite(MOTOR3LED,HIGH);
  digitalWrite(MOTOR4LED,HIGH);

  delay(100);

  digitalWrite(MOTOR1LED,LOW);
  digitalWrite(MOTOR2LED,LOW);
  digitalWrite(MOTOR3LED,LOW);
  digitalWrite(MOTOR4LED,LOW);

  delay(100);
}

void cutOffEngines(){
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);
  while (1) {
    ledBlink();
  }
}

void emergencyLanding(){
  int throtle = atoi(userIn.throttle);
  throtle = map(throtle,0,100,1000,2000);
  while (throtle>1000) {
    ESC1.writeMicroseconds(throtle - 50);
    ESC2.writeMicroseconds(throtle - 50);
    ESC3.writeMicroseconds(throtle - 50);
    ESC4.writeMicroseconds(throtle - 50);
    delay(1500);

    throtle -= 100;
  }
  cutOffEngines();
}

void loop() {
  if (radio.available()) {
    //char text[32] = "";
    radio.read(&userIn, sizeof(userIn));
    Serial.println(userIn.altitude);
    Serial.println(userIn.emergency_landing);
    Serial.println(userIn.engine_cut);
    Serial.println(userIn.hand_setup);
    Serial.println(userIn.pitch);
    Serial.println(userIn.roll);
    Serial.println(userIn.yaw);
    Serial.println(userIn.throttle);
    Serial.println(userIn.light);

    if (userIn.engine_cut == 1) {
      cutOffEngines();
    }

    if (strcmp(userIn.light,"01") == 0) {
      digitalWrite(MOTOR1LED,HIGH);
      digitalWrite(MOTOR2LED,HIGH);
      digitalWrite(MOTOR3LED,HIGH);
      digitalWrite(MOTOR4LED,HIGH);
    }else {
      digitalWrite(MOTOR1LED,LOW);
      digitalWrite(MOTOR2LED,LOW);
      digitalWrite(MOTOR3LED,LOW);
      digitalWrite(MOTOR4LED,LOW);
    }
    
    if (userIn.emergency_landing == 1){
      emergencyLanding();
    }

    int yaw = userIn.yaw;
    int pitch = userIn.pitch;
    int roll = userIn.roll;
    int throtle = atoi(userIn.throttle);
    throtle = constrain(throtle, 0, 100); 
    throtle = map(throtle,0,100,1000,2000);

    ESC1.writeMicroseconds(throtle);
    ESC2.writeMicroseconds(throtle);
    ESC3.writeMicroseconds(throtle);
    ESC4.writeMicroseconds(throtle);

    if(yaw > 900){
      ESC1.writeMicroseconds(throtle-100);
      ESC2.writeMicroseconds(throtle);
      ESC3.writeMicroseconds(throtle-100);
      ESC4.writeMicroseconds(throtle);
    }
    if(yaw < 100){
      ESC1.writeMicroseconds(throtle);
      ESC2.writeMicroseconds(throtle-100);
      ESC3.writeMicroseconds(throtle);
      ESC4.writeMicroseconds(throtle-100);
    }

    if(roll > 900){
      ESC1.writeMicroseconds(throtle-100);
      ESC2.writeMicroseconds(throtle);
      ESC3.writeMicroseconds(throtle);
      ESC4.writeMicroseconds(throtle-100);
    }
    if(roll < 100) {
      ESC1.writeMicroseconds(throtle);
      ESC2.writeMicroseconds(throtle-100);
      ESC3.writeMicroseconds(throtle-100);
      ESC4.writeMicroseconds(throtle);
    }

    if (pitch > 900) {
      ESC1.writeMicroseconds(throtle-100);
      ESC2.writeMicroseconds(throtle-100);
      ESC3.writeMicroseconds(throtle);
      ESC4.writeMicroseconds(throtle);
    }
    if (pitch < 100) {
      ESC1.writeMicroseconds(throtle);
      ESC2.writeMicroseconds(throtle);
      ESC3.writeMicroseconds(throtle-100);
      ESC4.writeMicroseconds(throtle-100);
    }
    digitalWrite(2,LOW);
  }else {
    Serial.println("NOPE");
    digitalWrite(2,HIGH);
  }
}
