#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 10, 11, 2, 3, 4, 5, 6, 7, /*enable=*/ 8, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 9, /*reset=*/ A1);

const byte SLAVE_ADDRESS = 8;

struct AccelerometerData {
  float x;
  float y;
};

struct GPS_Data {
  float latitude;
  float longitude;
  float altitude;
  float speed;
  int sattelites_number;
};

struct sensoryData {
  AccelerometerData accelorometer;
  GPS_Data gps;
  float temperature;
};
volatile sensoryData sensors;
volatile bool sensorsUpdated = false;

struct userInputs {
  char altitude[4];
  char throttle[4];
  char light[3];
  char throttle_change_disable[3];
  char hand_setup[3];
};
volatile userInputs userIn;
volatile bool userUpdated = false;

void setup(void) {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  u8g2.begin();
}

void receiveEvent(int howMany) {
  if (howMany == sizeof(userIn)) {
    Wire.readBytes((byte*)&userIn, sizeof(userIn));
    userUpdated = true;
  }
  else if (howMany == sizeof(sensors)) {
    Wire.readBytes((byte*)&sensors, sizeof(sensors));
    sensorsUpdated = true;
  }
  else {
    while (Wire.available()) Wire.read(); // flush buffer
  }
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

void loop(void) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_04b_03_tr);

  char buf[16];

  // --- GPS DATA ---
  u8g2.drawStr(0,10,"LAT:");
  if (sensorsUpdated) {
    dtostrf(sensors.gps.latitude, 7, 4, buf);
    u8g2.drawStr(23, 10, buf);
  }

  u8g2.drawStr(0,20,"LON:");
  if (sensorsUpdated) {
    dtostrf(sensors.gps.longitude, 7, 4, buf);
    u8g2.drawStr(23, 20, buf);
  }

  u8g2.drawStr(0,30,"ALT:");
  if (userUpdated) {
    dtostrf(sensors.gps.altitude, 3, 2, buf);
    u8g2.drawStr(22, 30, buf);
    if (strcmp(userIn.altitude,"999")){
      u8g2.drawStr(39,30,"/");
      u8g2.drawStr(45,30,userIn.altitude);
      u8g2.drawStr(60,30,"m");
    }
  }

  // --- THROTTLE ---
  u8g2.drawStr(75,10,"THR:");
  if (userUpdated) {
    u8g2.drawStr(95, 10, userIn.throttle);
    u8g2.drawStr(110, 10, "%");
    if (strcmp(userIn.throttle_change_disable,"01")==0) {
      u8g2.drawHLine(75, 11, 40);
    }
  }

  // --- SATTELITES --
  u8g2.drawStr(75,20,"SAT:");
  if (userUpdated) {
    dtostrf(sensors.gps.sattelites_number, 2, 0, buf);
    u8g2.drawStr(95, 20, buf);
  }

  // --- SPEED ---
  u8g2.drawStr(75,30,"SPD:");
  if (userUpdated) {
    dtostrf(sensors.gps.speed, 2, 2, buf);
    u8g2.drawStr(95, 30, buf);
    u8g2.drawStr(114, 30, "kph");
  }

  // --- LIGHTS ---
  u8g2.drawStr(75,40,"LGT:");
  if (userUpdated) {
    if (strcmp(userIn.light,"01")==0) {
      u8g2.drawStr(95, 40, "ON");
    } else {
      u8g2.drawStr(95, 40, "OFF");
    }
  }

  // --- TEMP ---
  u8g2.drawStr(75, 50, "TMP:");
  if (sensorsUpdated) {
    dtostrf(sensors.temperature, 5, 2, buf);
    u8g2.drawStr(95, 50, buf);
    u8g2.drawStr(117, 50, "C");
  }

  // --- ACCEL ---
  u8g2.drawStr(43,40,"X:");
  if (sensorsUpdated) {
    dtostrf(sensors.accelorometer.x, 5, 2, buf);
    u8g2.drawStr(50,40, buf);
  }
  u8g2.drawStr(43,50,"Y:");
  if (sensorsUpdated) {
    dtostrf(sensors.accelorometer.y, 5, 2, buf);
    u8g2.drawStr(50,50, buf);
  }

  // --- DRAW FACES ---
  if (((sensors.temperature>45)&&(sensors.temperature<50)) || (sensors.gps.sattelites_number<7)&&(sensors.gps.sattelites_number>3)) {
    drawNeutralFace();
  }else if ((sensors.temperature>49) || (sensors.gps.sattelites_number<4)) {
    drawSadFace();
  }else {
    drawSmileyFace();
  };

  // --- DRAW GPS ---
  if (sensors.gps.sattelites_number>2) {
    drawGPS();
  }

  // --- HAND MODE AND VERSION
  u8g2.setFont(u8g2_font_squeezed_b7_tr);
  if(strcmp(userIn.hand_setup,"01")==0){
    u8g2.drawStr(0,64,"LEFT HANDED");
    u8g2.drawStr(105, 64, "v0.21");
  }else {
    u8g2.drawStr(65,64,"RIGHT HANDED");
    u8g2.drawStr(0, 64, "v0.2");
  }

  u8g2.sendBuffer();

  delay(75);
}
