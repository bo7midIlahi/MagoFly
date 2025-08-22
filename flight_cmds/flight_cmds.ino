#include <Servo.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//nRF24L01
RF24 radio(14, 15);
const byte address[5] = {'F','L','G','H','T'};

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

struct userInputs {
  char altitude[4];
  char throttle[4];
  char light[3];
  char throttle_change_disable[3];
  char hand_setup[3];
  int emergency_landing;
  int engine_cut;
};
struct userInputs userIn;

#define ledPin 2
#define bluePin 19

void setup() {
  Serial.begin(115200);

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

  pinMode(ledPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
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
    radio.read(&userIn, sizeof(userIn));
    Serial.println("GOT DATA");
    digitalWrite(2,HIGH);
    delay(200);
    digitalWrite(2,LOW);
    delay(200);
  }else {
    Serial.println("NOPE");
    digitalWrite(19,HIGH);
    delay(200);
    digitalWrite(19,LOW);
    delay(200);
  }
}
