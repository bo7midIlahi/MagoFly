#include "communication.h"

// ============ Static variables ============
static CommRole deviceRole;
static WiFiUDP udp;
static IPAddress remoteIP;        // the other Pico's IP
static bool initialized = false;
static uint16_t localPort = 8888;

// ============ Public API ============

void commBegin(CommRole role, const char* ssid, const char* password, uint16_t port) {
  deviceRole = role;
  localPort = port;

  if (role == CommRole::REMOTE) {
    // Remote – create its own WiFi network
    WiFi.softAP(ssid, password);
    // Wait for AP to become ready
    while (WiFi.softAPIP() == IPAddress(0,0,0,0)) {
      delay(10);
    }
    // Drone will always be .2 (we assign static on drone side)
    remoteIP = IPAddress(192, 168, 4, 2);
  }
  else {  // DRONE
    // Disable DHCP – set static IP BEFORE connecting
    IPAddress localIP(192, 168, 4, 2);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(localIP, gateway, subnet);
    
    // Now connect (will use the static IP)
    WiFi.begin(ssid, password);
    
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < 10000) {
      delay(10);
    }
  
    // remoteIP will be updated when first packet arrives, but set a safe fallback
    remoteIP = IPAddress(192, 168, 4, 1);
  }
  udp.begin(localPort);
  initialized = true;
}

bool sendControl(const ControlPacket& ctrl) {
  if (!initialized || deviceRole != CommRole::REMOTE) return false;
  
  udp.beginPacket(remoteIP, localPort);
  udp.write((const uint8_t*)&ctrl, sizeof(ctrl));
  return udp.endPacket() == 1;
}

bool sendTelemetry(const TelemetryPacket& telem) {
  if (!initialized || deviceRole != CommRole::DRONE) return false;
  if (remoteIP == IPAddress(0,0,0,0)) return false;   // no known remote yet

  udp.beginPacket(remoteIP, localPort);
  udp.write((const uint8_t*)&telem, sizeof(telem));
  return udp.endPacket() == 1;
}

bool receiveControl(ControlPacket& ctrl) {
  if (!initialized || deviceRole != CommRole::DRONE) return false;
  
  int pktSize = udp.parsePacket();
  if (pktSize != sizeof(ControlPacket)) return false;

  udp.read((uint8_t*)&ctrl, sizeof(ctrl));
  // Update remote IP to the sender (should be the remote controller)
  remoteIP = udp.remoteIP();
  return true;
}

bool receiveTelemetry(TelemetryPacket& telem) {
  if (!initialized || deviceRole != CommRole::REMOTE) return false;

  int pktSize = udp.parsePacket();
  if (pktSize != sizeof(TelemetryPacket)) return false;

  udp.read((uint8_t*)&telem, sizeof(telem));
  // Optional: you could update remoteIP here as well
  return true;
}

bool commIsConnected() {
  if (!initialized) return false;
  if (deviceRole == CommRole::REMOTE) {
    return WiFi.softAPgetStationNum() > 0;
  } else {
    return WiFi.status() == WL_CONNECTED;
  }
}

IPAddress commRemoteIP() {
  return remoteIP;
}