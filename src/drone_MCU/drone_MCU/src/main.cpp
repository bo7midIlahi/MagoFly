#include <communication.h>

ControlPacket receivedCtrl;

void setup() {
    Serial.begin(115200);
    pinMode(16,OUTPUT);
    commBegin(CommRole::DRONE, "DroneRemote", "12345678");
}

void loop() {
  if (!commIsConnected()) {
    // Not connected – blink LED to indicate
    Serial.println("Not connected to remote");
    digitalWrite(16, HIGH);
    delay(500);
    digitalWrite(16, LOW);
    delay(500);
    return;
  }

  if (receiveControl(receivedCtrl)) {
    // Map received data to motor commands
    // receivedCtrl.roll, pitch, yaw, throttle, FLAGS, etc.
    //sio_hw->gpio_clr = (1 << 16);
    digitalWrite(16, LOW); 
    Serial.printf("ROLL: %d\tPITCH: %d\tYAW: %d\tTHROTTLE: %d\tALTITUTDE: %d",
      receivedCtrl.roll, receivedCtrl.pitch,
      receivedCtrl.yaw, receivedCtrl.throttle, receivedCtrl.altitude);
      Serial.printf("FLAGS: ");
      Serial.println(receivedCtrl.FLAGS, BIN);
  }else
  {
    // Toggle GPIO 16 instantly using the hardware XOR register
    Serial.println("No DATA");
    digitalWrite(16, HIGH);
  }
  
/*
  // Send telemetry (every 100 ms)
  static unsigned long lastTelem = 0;
  if (millis() - lastTelem > 100) {
    TelemetryPacket telem;
    telem.battery     = getBatteryPercent();
    telem.altitude_cm = getAltitudeCm();
    telem.rssi        = WiFi.RSSI();
    telem.errors      = getErrorFlags();
    sendTelemetry(telem);
    lastTelem = millis();
  }
*/
  delay(100);   // keep loop fast
}