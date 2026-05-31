#include <communication.h>
#include <Servo.h>
#include <motor.h>
//defining pins of the ESCs
#define ESC1_PIN 2
#define ESC2_PIN 3
#define ESC3_PIN 4
#define ESC4_PIN 5
//defining servo ESCs
Servo ESC1; 
Servo ESC2; 
Servo ESC3; 
Servo ESC4;

//define throt
uint16_t THROTTLE;

ControlPacket receivedCtrl;

void attach(){
  ESC1.attach(ESC1_PIN, 1000, 2000); 
  ESC2.attach(ESC2_PIN, 1000, 2000); 
  ESC3.attach(ESC3_PIN, 1000, 2000); 
  ESC4.attach(ESC4_PIN, 1000, 2000); 

  Serial.printf("finished attach");
}

void armESC() {
  // Send minimum throttle to all ESCs (idle signal)
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);
  delay(2000);  // wait for ESC init beeps (typically 1‑2 sec)
  Serial.println("ESCs armed – idle");
}

void calibrateAllESCs() {
  Serial.println("CALIBRATING ESCs – PROPS MUST BE OFF!");
  delay(2000); // give time to read message

  // Step 1: Send maximum throttle (2000 µs)
  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  ESC3.writeMicroseconds(2000);
  ESC4.writeMicroseconds(2000);
  Serial.println("Sending HIGH (2000 µs) – waiting for beeps…");
  delay(4000); // wait for ESCs to acknowledge (usually 2-3 sec)

  // Step 2: Send minimum throttle (1000 µs)
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);
  Serial.println("Sending LOW (1000 µs) – calibration done.");
  delay(2000); // wait for confirmation beeps

  // Now ESCs are calibrated – they will arm next time they see low throttle
}

void setup() {
  Serial.begin(115200);
  pinMode(16,OUTPUT);
  attach();
  commBegin(CommRole::DRONE, "DroneRemote", "12345678");
  calibrateAllESCs();
  armESC();
  Serial.printf("finished setup");
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
    THROTTLE = constrain(map(receivedCtrl.throttle, 0, 100, 1000, 2000), 1000, 2000);
    Serial.print("THROTL: "); Serial.println(THROTTLE);

    setThrottle(THROTTLE);
    if (receivedCtrl.pitch>30000) moveForward(THROTTLE);
    if (receivedCtrl.pitch<100) moveForward(THROTTLE);
    if (receivedCtrl.roll>30000) yawLeft(THROTTLE);
    if (receivedCtrl.roll<100) rollRight(THROTTLE);
    if (receivedCtrl.yaw>3000) yawLeft(THROTTLE);
    if (receivedCtrl.pitch<100) yawRight(THROTTLE);
    if (CHECK_BIT(receivedCtrl.FLAGS,3)) KILL_ENGINES();
    if (CHECK_BIT(receivedCtrl.FLAGS,5)) returnToGround(THROTTLE);

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
  Serial.println("loop");
  delay(100);   // keep loop fast
}