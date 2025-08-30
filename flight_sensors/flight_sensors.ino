#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define RX and TX pins for the GPS module
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

Adafruit_MPU6050 mpu;

//defining the nRF24L01 Radio
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

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
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
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

  //sending what's read
  radio.write(&sensors, sizeof(sensors));
}
