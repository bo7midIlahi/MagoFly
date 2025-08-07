#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 10, 11, 2, 3, 4, 5, 6, 7, /*enable=*/ 8 /* A4 */, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 9 /* A3 */, /*reset=*/ /*10*/ A1);  // Remember to set R/W to 0 
const byte SLAVE_ADDRESS = 8;
char receivedData[32]; // Adjust size as needed

void setup(void) {
  Wire.begin(SLAVE_ADDRESS); // Join the I2C bus as slave with its address
  Wire.onReceive(receiveEvent); // Register the receive event handler
  Serial.begin(115200);
  u8g2.begin();
}

struct ReceivedData {
  char altitude[4];
  char throttle[4];
  char lights[3];
  char throttle_change[3];
  char handMode[3];
};

struct ReceivedData received_Data;

void splitMessage(char message[17]){
  char * ptr = strtok(message,",");
  Serial.print("ptr 1 :");
  Serial.println(ptr);
  strcpy(received_Data.altitude,ptr);
  Serial.println(received_Data.altitude);

  ptr = strtok(NULL,",");
  Serial.print("ptr 2 :");
  Serial.println(ptr);
  strcpy(received_Data.throttle,ptr);
  Serial.println(received_Data.throttle);

  ptr = strtok(NULL,",");
  Serial.print("ptr 3 :");
  Serial.println(ptr);
  strcpy(received_Data.lights,ptr);
  received_Data.lights[2] = '\0';
  Serial.println(received_Data.lights);

  ptr = strtok(NULL,",");
  Serial.print("ptr 4 :");
  Serial.println(ptr);
  strcpy(received_Data.throttle_change,ptr);
  received_Data.throttle_change[2] = '\0';
  Serial.println(received_Data.throttle_change);

  ptr = strtok(NULL,",");
  Serial.print("ptr 5 :");
  Serial.println(ptr);
  strcpy(received_Data.handMode,ptr);
  received_Data.handMode[2] = '\0';
  Serial.println(received_Data.handMode);
}

void receiveEvent(int howMany) {
  int i = 0;
  char message[17];
  while (Wire.available()) { // Loop while there are bytes available
    char c = Wire.read(); // Read a byte
    if (i < 16) { // Prevent buffer overflow
      message[i++] = c;
    }
  }

  message[16] = '\0'; // Null-terminate the received string
  Serial.print("Received: ");
  Serial.println(message);
  splitMessage(message);
  /*
  char * ptr = strtok(message,",");
  Serial.print("ptr 1 :");
  Serial.println(ptr);
  strcpy(received_Data.altitude,ptr);

  ptr = strtok(NULL,",");
  Serial.print("ptr 2 :");
  Serial.println(ptr);
  strcpy(received_Data.throttle,ptr);*/
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

void loop(void) {
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_04b_03_tr); // choose a suitable font

  //POSITION DATA
  u8g2.drawStr(0,10,"LAT:");  // write something to the internal memory
  u8g2.drawStr(20, 10, "35.0522N");

  u8g2.drawStr(0,20,"LON:");  // write something to the internal memory
  u8g2.drawStr(20, 20, "118.2437W");
  
  u8g2.drawStr(0,30,"ALT:");  // write something to the internal memory
  u8g2.drawStr(20, 30, received_Data.altitude);
  
  //FLIGHT DATA
  u8g2.drawStr(78,10,"THR:");  // write something to the internal memory
  u8g2.drawStr(98, 10, received_Data.throttle);
  u8g2.drawStr(113, 10, "%");
  if (strcmp(received_Data.throttle_change,"01")==0) {
    u8g2.drawHLine(78, 11, 40);
  }

  u8g2.drawStr(78,20,"SAT:");  // write something to the internal memory
  u8g2.drawStr(98, 20, "12");
  
  u8g2.drawStr(78,30,"BAT:");  // write something to the internal memory
  u8g2.drawStr(98, 30, "100%");

  u8g2.drawStr(78,40,"LGT:");  // write something to the internal memory
  if (strcmp(received_Data.lights,"01")==0) {
    u8g2.drawStr(98, 40, "ON");
    Serial.println("received_Data.lights ON");
  }else {
    u8g2.drawStr(98, 40, "OFF");
    Serial.println("received_Data.lights OFF");
  }

  //DRAWINGS
  drawSmileyFace();
  drawGPS();
  drawBattery();

  //BOTTOM ROW
  u8g2.setFont(u8g2_font_squeezed_b7_tr); // choose a suitable font
  if(strcmp(received_Data.handMode,"01")==0){
    u8g2.drawStr(0,64,"LEFT HANDED");  // write something to the internal memory
    u8g2.drawStr(100, 64, "v0.14");
  }else {
    u8g2.drawStr(65,64,"RIGHT HANDED");  // write something to the internal memory
    u8g2.drawStr(0, 64, "v0.14");
  }

  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(75);
}
