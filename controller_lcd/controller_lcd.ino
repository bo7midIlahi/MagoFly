#include "Communication.h"

//Board adresses for the communication
#define COLTROLLER_CMD_ADRESS 10
#define COLTROLLER_LCD_ADRESS 25

static Communication comms(COLTROLLER_LCD_ADRESS);

#include <Arduino.h>
#include <U8g2lib.h>

/*#include <nRF24L01.h>
#include <RF24.h>*/

/*
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
*/
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 0, 1, 2, 3, 4, 5, 6, 7, /*enable=*/ 8 /* A4 */, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 9 /* A3 */, /*reset=*/ /*10*/ A1);  // Remember to set R/W to 0 

//defining radio
/*
RF24 radio(A2, A3); // CE, CSN
const byte address[6] = "00001";

struct joystick {
  int x;
  int y;
  int btn;
};

struct data {
  struct joystick joystick;
  int throttle;
  int rotation;
  int light;
};
*/

void setup(void) {
  //Serial.begin(115200);
  
  /*radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();*/

  u8g2.begin();
}

void drawSmileyFace() {
  u8g2.drawCircle(10, 43, 10);
  u8g2.drawPixel(15, 40);
  u8g2.drawPixel(5, 40);
  u8g2.drawArc(10, 43, 6, 149, 236);
}

void drawNeutralFace() {
  u8g2.drawCircle(10, 43, 10);
  u8g2.drawPixel(15, 40);
  u8g2.drawPixel(5, 40);
  u8g2.drawLine(5, 45, 15, 45);
}

void drawSadFace() {
  u8g2.drawCircle(10, 43, 10);
  u8g2.drawPixel(15, 40);
  u8g2.drawPixel(5, 40);
  u8g2.drawArc(10, 51, 6, 25, 102);
}

void drawGPS() {
  u8g2.drawArc(32, 40, 7, 0, 128);
  u8g2.drawCircle(32, 40, 3);
  u8g2.drawLine(24, 40, 32, 52);
  u8g2.drawLine(32, 52, 39, 40);
}

void drawBattery() {
  u8g2.drawFrame(43, 33, 10, 20);
  int i=0;
  while (i<10) {
    u8g2.drawHLine(45, 50 - (1.7*i), 6);
    i += 2;
  }
}

/*char* getSubString(int param){
  char message = comms.GetStringMessage();
  static char token[10];

  memset(token, 0, sizeof(token));

  switch (param) {
    case 1:
      for (int i=0; i<4; i++) {
        token[i] = message[i];
      }
      break;
    
    case 2:
      for (int i=0; i<4; i++) {
        token[i] = message[i+5];
      }
      break;
  }

  return token;
}*/

void loop(void) {

/*
  struct data data;
  if (radio.available()) {
    radio.read(&data, sizeof(struct data));
  }

  /*preparng data to show on the lcd
  const char joystickX[10];
  sprintf(joystickX, "%d", data.joystick.x);
  
  const char joystickY[10];
  sprintf(joystickY, "%d", data.joystick.y);

  const char joystickBTN[10];
  sprintf(joystickBTN, "%d", data.joystick.btn);

  const char light[10];
  sprintf(light, "%d", data.light);

  const char throttle[10];
  sprintf(throttle, "%d", data.throttle);


  Serial.println("JOY X: ");
  Serial.println(data.joystick.x);
*/
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_04b_03_tr); // choose a suitable font

  //POSITION DATA
  u8g2.drawStr(0,10,"LAT:");  // write something to the internal memory
  u8g2.drawStr(20, 10, "35.0522N");

  u8g2.drawStr(0,20,"LON:");  // write something to the internal memory
  u8g2.drawStr(20, 20, "118.2437W");
  
  u8g2.drawStr(0,30,"ALT:");  // write something to the internal memory

  char message_received = comms.GetStringMessage();
  /*String msgString = String(message_received);
  String alt = msgString.substring(0,3);
  char alttt[4];
  alt.toCharArray(alttt, 4);*/
  u8g2.drawStr(20, 30, message_received);
  
  
  //FLIGHT DATA
  u8g2.drawStr(78,10,"THR:");  // write something to the internal memory
  //token = strtok(NULL,",");
  u8g2.drawStr(98, 10, "100%");

  u8g2.drawStr(78,20,"SAT:");  // write something to the internal memory
  u8g2.drawStr(98, 20, "12");
  
  u8g2.drawStr(78,30,"BAT:");  // write something to the internal memory
  u8g2.drawStr(98, 30, "100%");

  u8g2.drawStr(78,40,"LGT:");  // write something to the internal memory
  u8g2.drawStr(98, 40, "OFF");

  //DRAWINGS
  drawSmileyFace();
  drawGPS();
  drawBattery();

  //BOTTOM ROW
  u8g2.setFont(u8g2_font_squeezed_b7_tr); // choose a suitable font
  u8g2.drawStr(0,64,"LEFT HANDED");  // write something to the internal memory
  u8g2.drawStr(100, 64, "v0.14");

  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(75);
}