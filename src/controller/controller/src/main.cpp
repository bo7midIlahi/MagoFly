#include <Arduino.h>
#include <U8g2lib.h>
#include <lcd.h>

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 7, /* CS=*/ 9, /* reset=*/ 8);

//uint8_t draw_state = 0; //remove comment to debug

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    drawText();
    drawValues(9999,8888,7777,100,555);
    //testDraw(draw_state); //remove comment to debug
  } while (u8g2.nextPage());

  /*draw_state++; //remove comment to debug
  if (draw_state >= 12 * 8)
    draw_state = 0;
  */
  delay(100);
}