#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

// Define RX and TX pins for the GPS module
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

Adafruit_MPU6050 mpu;

struct AccelerometerData{
  float x;
  float y;
};

struct GPS_Data{
  float latitude;
  float longitude;
  float altitude;
  float speed;
  int sattelites_number;
};

struct sensoryData{
  struct AccelerometerData accelorometer;
  struct GPS_Data gps;
  float temperature; // temperature in °C
};

struct sensoryData sensors;

void setup() {
  Serial.begin(115200);
  mpu.begin();
  ss.begin(GPSBaud);
  ina219.begin();

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  ina219.setCalibration_16V_400mA();
}

void loop(){
  //getting accelerometer readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sensors.accelorometer.x = a.acceleration.x;
  sensors.accelorometer.y = a.acceleration.y;
  sensors.temperature = temp.temperature;

  //getting GPS readings
  sensors.gps.altitude = gps.altitude.meters();
  sensors.gps.latitude = gps.location.lat();
  sensors.gps.longitude = gps.location.lng();
  sensors.gps.sattelites_number = gps.satellites.value();
  sensors.gps.speed = gps.speed.kmph();

  //getting ina219 readings
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  //printing accelorometer data
  Serial.println("ACCEL: ");
  Serial.println(sensors.accelorometer.x);
  Serial.println(sensors.accelorometer.y);
  Serial.println(sensors.temperature);

  //printing gps data
  Serial.println("GPS: ");
  Serial.println(sensors.gps.altitude);
  Serial.println(sensors.gps.latitude);
  Serial.println(sensors.gps.longitude);
  Serial.println(sensors.gps.sattelites_number);
  Serial.println(gps.date.month());

  //printing ina219 data
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(1000);
}
