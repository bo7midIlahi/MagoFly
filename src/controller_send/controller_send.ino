/*#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <Wire.h>

// nRF24L01
RF24 radio(4, 5);
const byte address[6] = "FLGHT";

const byte radio_arduino = 3;

// structure of the user input
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

void receiveEvent(int howMany) {
  if (howMany == sizeof(userIn)) {
    for (int i = 0; i < sizeof(userIn); i++) {
      ((byte*)&userIn)[i] = Wire.read();
    }
    Serial.println("DATA ACQUIRED");
  }
}

void setup() {
  Serial.begin(115200);   // UART link
  while (!Serial);        // Wait for serial connection (Pro Micro)

  Wire.begin(radio_arduino);
  Wire.onReceive(receiveEvent);

  // nRF24 setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();  // transmitter mode

  pinMode(8, OUTPUT);

  Serial.println("UART → RF24 bridge ready");
}

void loop() {
  bool ok = radio.write(&userIn, sizeof(userIn));

  // debug output
  Serial.print("Altitude: ");
  Serial.println(userIn.altitude);
  Serial.print("RF24 send: ");
  Serial.println(ok ? "OK" : "FAILED");

  delay(50);
  
}*/
/*
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 7); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(2,OUTPUT);
}

void loop() {
  char txt[] = "HELLO WORLD";
  radio.write(&txt,sizeof(txt));
  delay(75);
}*/


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <Wire.h>

// nRF24L01
RF24 radio(8, 7);
const byte address[6] = "00010";

const byte radio_arduino = 3;

// structure of the user input
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

void receiveEvent(int howMany) {
  if (howMany == sizeof(userIn)) {
    for (int i = 0; i < sizeof(userIn); i++) {
      ((byte*)&userIn)[i] = Wire.read();
    }
    Serial.println("DATA ACQUIRED");
  }
}

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  pinMode(2,OUTPUT);

  Wire.begin(radio_arduino);
  Wire.onReceive(receiveEvent);
}

void printUserInput() {
  Serial.print("altitude: ");
  Serial.println(userIn.altitude);

  Serial.print("throttle: ");
  Serial.println(userIn.throttle);

  Serial.print("light: ");
  Serial.println(userIn.light);

  Serial.print("throttle_change_disable: ");
  Serial.println(userIn.throttle_change_disable);

  Serial.print("hand_setup: ");
  Serial.println(userIn.hand_setup);

  Serial.print("Emergency landing: ");
  Serial.println(userIn.emergency_landing);

  Serial.print("Engine cut: ");
  Serial.println(userIn.engine_cut);

  Serial.print("Yaw: ");
  Serial.println(userIn.yaw);

  Serial.print("Pitch: ");
  Serial.println(userIn.pitch);

  Serial.print("Roll: ");
  Serial.println(userIn.roll);
}

void loop() {
  printUserInput();
  //char txt[] = "HELLO WORLD";
  radio.write(&userIn,sizeof(userIn));
  delay(75);
}
