#include <Arduino.h>
#include <Wire.h>
#include <ADS1X15.h>

ADS1115 ADS(0x48);

#define JOYSTICK1_BTN 15
#define JOYSTICK2_BTN 14

#define BTN_RED 6
#define BTN_GREEN 9
#define BTN_BLUE 8

void setup() {
  Serial.begin(115200);
  Wire.begin();  // make sure to start Wire

  if (!ADS.begin()) {
    Serial.println("ADS1115 not found. Check wiring and address!");
    while (1);
  }
  ADS.setGain(0);  // ±6.144V, 1 bit = 0.1875mV
  Serial.println("ADS1115 ready.");

  pinMode(JOYSTICK1_BTN, INPUT);
  pinMode(JOYSTICK2_BTN, INPUT);

  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);

}

void loop() {
  int16_t roll  = ADS.readADC(0);
  int16_t pitch = ADS.readADC(1);
  int16_t yaw   = ADS.readADC(2);
  int16_t thr   = ADS.readADC(3);

  // convert each to voltage
  float v0 = ADS.toVoltage(roll);
  float v1 = ADS.toVoltage(pitch);
  float v2 = ADS.toVoltage(yaw);
  float v3 = ADS.toVoltage(thr);

  Serial.print("Roll: ");   Serial.print(roll);   Serial.print("\t"); Serial.println(v0, 3);
  Serial.print("Pitch: "); Serial.print(pitch); Serial.print("\t"); Serial.println(v1, 3);
  Serial.print("Yaw: ");   Serial.print(yaw);   Serial.print("\t"); Serial.println(v2, 3);
  Serial.print("Thr: ");   Serial.print(thr);   Serial.print("\t"); Serial.println(v3, 3);
  Serial.print("BTN1: ");
  if (digitalRead(JOYSTICK1_BTN))
  {
    Serial.println("HIGH");
  }else
  {
    Serial.println("LOW");
  }
  
  
  Serial.print("BNT2: ");
  if (digitalRead(JOYSTICK2_BTN))
  {
    Serial.println("HIGH");
  }else
  {
    Serial.println("LOW");
  }

  Serial.print("BTN_RED: ");
  if (digitalRead(BTN_RED))
  {
    Serial.println("HIGH");
  }else
  {
    Serial.println("LOW");
  }

  Serial.print("BTN_GREEN: ");
  if (digitalRead(BTN_GREEN))
  {
    Serial.println("HIGH");
  }else
  {
    Serial.println("LOW");
  }

  Serial.print("BTN_BLUE: ");
  if (digitalRead(BTN_BLUE))
  {
    Serial.println("HIGH");
  }else
  {
    Serial.println("LOW");
  }

  Serial.println();

  delay(1000);
}