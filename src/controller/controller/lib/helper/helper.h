#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Extern declarations – the actual objects live in main.cpp
extern Adafruit_ADS1115 ADS;
extern struct Collection userCollection;

// The struct definition (kept here so everyone sees it)
struct Collection {
  int roll;
  int pitch;
  int yaw;
  int throttle;
  uint8_t altitude;
  byte FLAGS;   // bit‑mapped flags
};

void getUserJoysticks(int X1, int Y1, int X2, int Y2);

#endif