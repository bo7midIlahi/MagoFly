#include <Arduino.h>
#include <U8g2lib.h>
#include <lcd.h>
#include <helper.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <communication.h>

Adafruit_ADS1115 ADS;  /* Use this for the 16-bit ADS1115 ADC */

// Joystick pins
#define JOYSTICK1X   1
#define JOYSTICK1Y   0
#define JOYSTICK1BTN 14
#define JOYSTICK2X   3
#define JOYSTICK2Y   2
#define JOYSTICK2BTN 15

// Button pins (all zero for now)
#define HAND_MODE          21
#define ALTITUDE_CONTROL   20
#define LIGHT_ENABLE       19

// Potentiometer
#define POTENTIOMETER 26

//Toggle Wsitch
#define TOGGLE_SWITCH 27

//define connection status
#define CONNECTED false
// One and only definition of userCollection
struct Collection userCollection;

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 7, /* CS=*/ 9, /* reset=*/ 8);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 6, 7, 9, 8);

void setup(void) {
  Serial.begin(115200);        // Always specify a baud rate
  //u8g2.begin();
  //u8g2.setBusClock(8000000);
  ADS.setGain(GAIN_ONE); // Set gain to 1 for a +/- 4.096V range

  if (!ADS.begin()) {
    Serial.println("Failed to initialize ADS. Check wiring!");
    while (1);
  }           // 0=6.144V, 1=4.096V, 2=2.048V …

  //PINS
  pinMode(JOYSTICK1BTN, INPUT_PULLUP);
  pinMode(JOYSTICK2BTN, INPUT_PULLUP);
  pinMode(HAND_MODE, INPUT_PULLUP);
  pinMode(ALTITUDE_CONTROL, INPUT_PULLUP);
  pinMode(LIGHT_ENABLE, INPUT_PULLUP);
  pinMode(TOGGLE_SWITCH, INPUT_PULLDOWN);

  //setup wifi
  commBegin(CommRole::REMOTE, "DroneRemote", "12345678");
}

void sendPacket(){
  ControlPacket ctrl;
  ctrl.roll      = userCollection.roll;
  ctrl.pitch     = userCollection.pitch;
  ctrl.yaw       = userCollection.yaw;
  ctrl.throttle  = userCollection.throttle;
  ctrl.altitude  = userCollection.altitude;
  ctrl.FLAGS     = userCollection.FLAGS;
  Serial.print("SENT : ");
  Serial.println(sendControl(ctrl));
}

void loop(void) {
  getUserJoysticks(digitalRead(JOYSTICK1BTN),digitalRead(JOYSTICK2BTN),JOYSTICK1X, JOYSTICK1Y, JOYSTICK2X, JOYSTICK2Y);
  userCollection.altitude = map(analogRead(POTENTIOMETER),0,4096,0,1024);
  setFLAGS(digitalRead(HAND_MODE),digitalRead(ALTITUDE_CONTROL),digitalRead(LIGHT_ENABLE),digitalRead(TOGGLE_SWITCH));
  Serial.printf("ROLL: %d\tPITCH: %d\tYAW: %d\tTHROTTLE: %d\tTHROT: %d\tALTITUTDE: %d\tBTN1: %d\tBTN2: %d\n",
                userCollection.roll, userCollection.pitch,
                userCollection.yaw, userCollection.throttle,ADS.readADC_SingleEnded(JOYSTICK1Y),
                userCollection.altitude, digitalRead(JOYSTICK1BTN),digitalRead(JOYSTICK2BTN));
  Serial.printf("FLAGS: ");
  Serial.print(userCollection.FLAGS, BIN);
  Serial.print("\n");

  sendPacket();
  delay(100);
}

void recievePacket(){
  TelemetryPacket telem;
  if (receiveTelemetry(telem)) {
    Serial.println("RECEIVED DATA");
  }
}

void setup1() {
  u8g2.begin();

  // Play boot animation for ~2 seconds
  for (int i = 0; i < 25; i++) {      // 25 frames
    u8g2.clearBuffer();
    drawBootLogo(i * 10);
    u8g2.sendBuffer();
    delay(80);
  }

  // Clear screen and prepare for main HUD
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  recievePacket();
}

void loop1(){
  u8g2.clearBuffer();

  drawText(CONNECTED);
  drawValues(userCollection.roll, userCollection.pitch, userCollection.yaw, userCollection.throttle, CHECK_BIT(userCollection.FLAGS,1), CHECK_BIT(userCollection.FLAGS,2));
  //drawSmileyFace();
  if (CHECK_BIT(userCollection.FLAGS,5)) drawBlinkingLanding();
  if (CHECK_BIT(userCollection.FLAGS,3)) drawEngineCut();
  commIsConnected() ? CONNECTED==true : CONNECTED==false;
  
  u8g2.sendBuffer();
};