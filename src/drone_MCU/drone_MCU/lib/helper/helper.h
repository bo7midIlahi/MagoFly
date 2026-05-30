#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>
#include <Wire.h>
//#include <Adafruit_ADS1X15.h>

//MACROS
#define SET_BIT(byte, n)   ((byte) |= (1ULL << (n)))
#define CLEAR_BIT(byte, n) ((byte) &= ~(1ULL << (n)))
#define TOGGLE_BIT(byte, n)((byte) ^= (1ULL << (n)))
#define CHECK_BIT(byte, n) ((byte >> n) & 1)

// Extern declarations – the actual objects live in main.cpp
//extern Adafruit_ADS1115 ADS;
extern struct Collection userCollection;

// The struct definition (kept here so everyone sees it)
struct Collection {
  int roll;
  int pitch;
  int yaw;
  int throttle;
  uint8_t altitude;
  byte FLAGS;   // bit‑mapped flags
  /*
  1st LSB -> Hand_Mode
  2nd LSB -> Light Enable
  3rd LSB -> AltitudeCTRL Enable
  4th LSB -> Kill Engines
  5th LSB -> Return To Ground
  */
};

//void getUserJoysticks(int joystickbtn1, int joystickbtn2,int X1, int Y1, int X2, int Y2);
//void setFLAGS(int handmode, int alt_ctrl, int light_enable,int engine_kill);

#endif