#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <helper.h>

// The U8g2 object is defined in main.cpp – this makes it visible to the library
extern U8G2_ST7920_128X64_F_SW_SPI u8g2;

// The only function that needs to be public
void testDraw(uint8_t state);
void drawText();
void drawValues(int roll, int pitch, int yaw, int throttle, int alt, int light);
void drawSmileyFace();
void drawBlinkingLanding();
void drawEngineCut();
void drawBootLogo(uint8_t angle);   // angle for propeller rotation

#endif