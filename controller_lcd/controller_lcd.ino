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
  int emergency_landing;
  int engine_cut;
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

void drawGPS(int gpsNumber) {
  if (gpsNumber>3) {
    u8g2.drawArc(32, 40, 7, 0, 128);
    u8g2.drawCircle(32, 40, 3);
    u8g2.drawLine(24, 40, 32, 52);
    u8g2.drawLine(32, 52, 39, 40);
  }else if ((gpsNumber<4) && (gpsNumber>0)) {
    static bool blink = false;
    static unsigned long lastToggle = 0;
    unsigned long now = millis();

    if (now - lastToggle >= 250) {
      blink = !blink;
      lastToggle = now;
    }

    if (blink) {
      // draw landing icon
      u8g2.drawArc(32, 40, 7, 0, 128);
      u8g2.drawCircle(32, 40, 3);
      u8g2.drawLine(24, 40, 32, 52);
      u8g2.drawLine(32, 52, 39, 40);
    }else {
      asm volatile("NOP");
    }
  }
}

void drawBlinkingLanding() {
  static bool blink = false;
  static unsigned long lastToggle = 0;
  unsigned long now = millis();

  // toggle every 500 ms
  if (now - lastToggle >= 500) {
    blink = !blink;
    lastToggle = now;
  }

  if (blink) {
    // draw landing icon
    u8g2.drawLine(61, 5, 61, 15);
    u8g2.drawLine(65, 5, 65, 15);
    u8g2.drawLine(61, 5, 65, 5);
    u8g2.drawLine(61, 15, 58, 12);
    u8g2.drawLine(65, 15, 68, 12);
    u8g2.drawLine(68, 12, 70, 14);
    u8g2.drawLine(58, 12, 56, 14);
    u8g2.drawLine(56, 14, 63, 21);
    u8g2.drawLine(70, 14, 63, 21);
  }
}

void drawEngineCut() {
  static bool blink = false;
  static unsigned long lastToggle = 0;
  unsigned long now = millis();

  if (now - lastToggle >= 1000) {
    blink = !blink;
    lastToggle = now;
  }

  if (blink) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_squeezed_b7_tr);
    //draws drone frame
    u8g2.drawCircle(30, 10, 10);
    u8g2.drawLine(30, 3, 30, 15);
    u8g2.drawPixel(30,18);

    u8g2.drawCircle(95, 10, 10);
    u8g2.drawLine(95, 3, 95, 15);
    u8g2.drawPixel(95,18);

    u8g2.drawCircle(30, 47, 10);
    u8g2.drawLine(30, 40, 30, 50);
    u8g2.drawPixel(30,53);

    u8g2.drawCircle(95, 47, 10);
    u8g2.drawLine(95, 40, 95, 50);
    u8g2.drawPixel(95,53);

    u8g2.drawRFrame(43, 20, 40, 20, 5);
    u8g2.drawLine(38, 18, 44, 22);
    u8g2.drawLine(86, 43, 80, 38);
    u8g2.drawLine(38, 40, 45, 38);
    u8g2.drawLine(80, 21, 88, 17);
    
    u8g2.drawStr(38, 64, "ENGINES OFF");

    /*
    //draws triangle warning sign
    u8g2.drawLine(5, 35, 25, 35);
    u8g2.drawLine(102, 35, 122, 35);
    u8g2.drawLine(5, 35, 15, 15);
    u8g2.drawLine(102, 35, 113, 15);
    u8g2.drawLine(15, 15, 25, 35);
    u8g2.drawLine(113, 15, 122, 35);
    u8g2.drawLine(15, 20, 15, 30);
    u8g2.drawLine(113, 20, 113, 30);
    u8g2.drawPixel(15,33);
    u8g2.drawPixel(113,33);
    */
  }
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

  // --- HAND MODE AND VERSION
  u8g2.setFont(u8g2_font_squeezed_b7_tr);
  if(strcmp(userIn.hand_setup,"01")==0){
    u8g2.drawStr(0,64,"LEFT HANDED");
    u8g2.drawStr(105, 64, "v0.21");
  }else {
    u8g2.drawStr(65,64,"RIGHT HANDED");
    u8g2.drawStr(0, 64, "v0.2");
  }

  // --- DRAW GPS ---
  /*if (sensors.gps.sattelites_number>3) {
    drawGPS();
  }*/
  drawGPS(sensors.gps.sattelites_number);

  // --- DRAW LANDING ---
  if (userIn.emergency_landing==1){
    drawBlinkingLanding();
  }

  //--- Draw Engine Cut ---
  if (userIn.engine_cut==1){
    drawEngineCut();
  }

  u8g2.sendBuffer();

  delay(75);
}
