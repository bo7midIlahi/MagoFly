#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// nRF24L01
RF24 radio(4, 5);
byte addresses[6] = "FLGHT";  // 5 chars + null = 6 bytes

// structure of the user input
struct userInputs {
  char altitude[4];
  char throttle[4];
  char light[3];
  char throttle_change_disable[3];
  char hand_setup[3];
  int emergency_landing;
  int engine_cut;
  int yaw;
  int pitch;
  int roll;
};
struct userInputs userIn;

void setup() {
  Serial.begin(115200);   // UART link
  while (!Serial);        // Wait for serial connection (Pro Micro)

  // nRF24 setup
  radio.begin();
  radio.openWritingPipe(addresses);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();  // transmitter mode

  pinMode(8, OUTPUT);

  Serial.println("UART → RF24 bridge ready");
}

void loop() {
  // Check if enough bytes for a full struct are available on UART
  if (Serial.available() >= sizeof(userIn)) {
    Serial.readBytes((byte*)&userIn, sizeof(userIn));  // fill struct

    // flash LED when data received
    digitalWrite(8, HIGH);

    // forward over RF24
    bool ok = radio.write(&userIn, sizeof(userIn));

    // debug output
    Serial.print("Altitude: ");
    Serial.println(userIn.altitude);
    Serial.print("RF24 send: ");
    Serial.println(ok ? "OK" : "FAILED");

    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
