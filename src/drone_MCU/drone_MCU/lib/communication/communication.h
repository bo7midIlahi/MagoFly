#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "helper.h"   // Collection is defined here

// Packet structures
struct __attribute__((packed)) ControlPacket {
  // Using Collection directly – the struct already contains all needed fields
  int roll;
  int pitch;
  int yaw;
  int throttle;
  uint8_t altitude;
  byte FLAGS;
};

struct __attribute__((packed)) TelemetryPacket {
  uint8_t battery;      // 0-100%
  int16_t altitude_cm;  // signed, in cm
  uint8_t rssi;         // 0-255 signal strength
  uint8_t errors;       // bit-coded warnings
};

// Role of this device
enum class CommRole {
  REMOTE,   // Sends control, receives telemetry
  DRONE     // Receives control, sends telemetry
};

// Initialization
void commBegin(CommRole role, const char* ssid, const char* password, uint16_t port = 8888);

// Sending functions (only the appropriate side should call these)
bool sendControl(const ControlPacket& ctrl);
bool sendTelemetry(const TelemetryPacket& telem);

// Receiving functions (non-blocking, return true if a packet was available)
bool receiveControl(ControlPacket& ctrl);
bool receiveTelemetry(TelemetryPacket& telem);

// Helper
bool commIsConnected();
IPAddress commRemoteIP();

#endif