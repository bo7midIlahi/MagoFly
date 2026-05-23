#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <U8g2lib.h>

// The U8g2 object is defined in main.cpp – this makes it visible to the library
extern U8G2_ST7920_128X64_1_SW_SPI u8g2;

// The only function that needs to be public
void draw(uint8_t state);

#endif