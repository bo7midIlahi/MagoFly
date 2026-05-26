#include <Arduino.h>
#include <U8g2lib.h>
#include <lcd.h>
#include <helper.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ADS;  /* Use this for the 16-bit ADS1115 ADC */

// Joystick pins
#define JOYSTICK1X   0
#define JOYSTICK1Y   1
#define JOYSTICK1BTN 14
#define JOYSTICK2X   2
#define JOYSTICK2Y   3
#define JOYSTICK2BTN 15

// Button pins (all zero for now)
#define HAND_MODE          0
#define ALTITUDE_CONTROL   0
#define LIGHT_ENABLE       0

// Potentiometer
#define POTENTIOMETER 26

// One and only definition of userCollection
struct Collection userCollection;

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 6, 7, 9, 8);

void setup(void) {
  Serial.begin(115200);        // Always specify a baud rate
  u8g2.begin();
  u8g2.setBusClock(8000000);
  ADS.setGain(GAIN_ONE); // Set gain to 1 for a +/- 4.096V range

  if (!ADS.begin()) {
    Serial.println("Failed to initialize ADS. Check wiring!");
    while (1);
  }           // 0=6.144V, 1=4.096V, 2=2.048V …
}

void loop(void) {
  u8g2.firstPage();
  do {
    drawText();
    drawValues(9999, 8888, 7777, 100, 555, userCollection.altitude);
    getUserJoysticks(JOYSTICK1X, JOYSTICK1Y, JOYSTICK2X, JOYSTICK2Y);
    userCollection.altitude = map(analogRead(POTENTIOMETER),0,4096,0,1024);
    Serial.printf("ROLL: %d\tPITCH: %d\tYAW: %d\tTHROTTLE: %d\t ALTITUTDE: %d\n",
                  userCollection.roll, userCollection.pitch,
                  userCollection.yaw, userCollection.throttle,
                  userCollection.altitude);
  } while (u8g2.nextPage());
  delay(100);
}