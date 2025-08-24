/*#include <Servo.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//nRF24L01
RF24 radio(8, 7);
const byte address[6] = "00010";

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

#define MOTOR1LED 15
#define MOTOR2LED 16
#define MOTOR3LED 17
#define MOTOR4LED 18

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

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);

  ESC1.attach(3,1000,2000);
  ESC2.attach(5,1000,2000);
  ESC3.attach(6,1000,2000);
  ESC4.attach(9,1000,2000);

  //calibration
  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  ESC3.writeMicroseconds(2000);
  ESC4.writeMicroseconds(2000);

  delay(3000);

  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);

  delay(3000);

  Serial.println("DONE CALIBRATION");

  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //setup pins
  pinMode(MOTOR1LED,OUTPUT);
  pinMode(MOTOR2LED,OUTPUT);
  pinMode(MOTOR3LED,OUTPUT);
  pinMode(MOTOR4LED,OUTPUT);
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

void loop() {
  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    digitalWrite(2,HIGH);

    digitalWrite(MOTOR1LED,HIGH);
    digitalWrite(MOTOR2LED,HIGH);
    digitalWrite(MOTOR3LED,HIGH);
    digitalWrite(MOTOR4LED,HIGH);

    Serial.println(userIn.altitude);
    Serial.println(userIn.emergency_landing);
    Serial.println(userIn.engine_cut);
    Serial.println(userIn.hand_setup);
    Serial.println(userIn.pitch);
    Serial.println(userIn.roll);
    Serial.println(userIn.yaw);
    Serial.println(userIn.throttle);
  }else {
    Serial.println("NOPE");
    digitalWrite(2,HIGH);
    digitalWrite(MOTOR1LED,HIGH);
    digitalWrite(MOTOR2LED,HIGH);
    digitalWrite(MOTOR3LED,HIGH);
    digitalWrite(MOTOR4LED,HIGH);
    delay(200);
    digitalWrite(2,LOW);
    digitalWrite(MOTOR1LED,LOW);
    digitalWrite(MOTOR2LED,LOW);
    digitalWrite(MOTOR3LED,LOW);
    digitalWrite(MOTOR4LED,LOW);
    delay(200);
  }
}*/

/*
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(2,OUTPUT);
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    digitalWrite(2,HIGH);
  }else {
    Serial.println("NOP");
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    delay(100);
  }
  delay(200);
}
*/



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

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  pinMode(2,OUTPUT);
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
    digitalWrite(2,HIGH);
  }else {
    Serial.println("NOP");
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    delay(100);
  }
}
