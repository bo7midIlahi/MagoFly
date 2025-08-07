#include <Wire.h>
const byte SLAVE_ADDRESS = 8; // Choose an address for your slave Arduino

//user inputs
#define LEFT_JOYSTICK_X_PIN A3
#define LEFT_JOYSTICK_Y_PIN A2
#define LEFT_JOYSTICK_BTN_PIN 3

#define RIGHT_JOYSTICK_X_PIN A7
#define RIGHT_JOYSTICK_Y_PIN A6
#define RIGHT_JOYSTICK_BTN_PIN 2

#define POTENTIOMETER_PIN A1

#define SWITCH_HAND_MODE_PIN 6
#define LIGHT_ENABLE_PIN 5
#define ALTITUDE_CONTROL_ENABLE_PIN 4

#define ENGINE_CUT_PIN 14

//defining FLAGS
struct FLAGS{
  int hand_setup; //1=right && -1==left
  int lights; //-1=off && 1=on
  int altitude_control; //-1=off && 1=on
  int throttle_change_disable; //enable the user to change throttle or not [(-1=can change) && (1=can't change)]
  int emergency_landing; //-1=off && 1=on
  int engine_cut; //instantly kill the engines -1=off && 1=on
};
struct FLAGS flags; 

//defining data to send via nrf24 to drone
struct DataToSend{
  int yaw;
  int throttle;
  int rth;
  int roll;
  int pitch;
  int throtctr;

};
struct DataToSend data_to_send;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(LEFT_JOYSTICK_BTN_PIN, INPUT_PULLUP);
  pinMode(RIGHT_JOYSTICK_BTN_PIN, INPUT_PULLUP);

  pinMode(SWITCH_HAND_MODE_PIN, INPUT);
  pinMode(LIGHT_ENABLE_PIN, INPUT);
  pinMode(ALTITUDE_CONTROL_ENABLE_PIN, INPUT);

  pinMode(ENGINE_CUT_PIN, INPUT);

  //setting up the flags
  flags.altitude_control = -1;
  flags.emergency_landing = -1;
  flags.engine_cut = -1;
  flags.hand_setup = 1;
  flags.lights = -1;
  flags.throttle_change_disable = -1;

  //setting up the data to send
  data_to_send.yaw = 0;
  data_to_send.throttle = 0;
  data_to_send.rth = -1;
  data_to_send.roll = 0;
  data_to_send.pitch = 0;
  data_to_send.throtctr = -1;
}

void loop() {
  if (flags.hand_setup==1){
    data_to_send.yaw = analogRead(LEFT_JOYSTICK_X_PIN);
    data_to_send.rth = digitalRead(LEFT_JOYSTICK_BTN_PIN);
    if ((flags.throttle_change_disable == -1)) {
      if ((analogRead(LEFT_JOYSTICK_Y_PIN)>1000) && (data_to_send.throttle<100)) {
        data_to_send.throttle += 1 ;
      }
      if ((analogRead(LEFT_JOYSTICK_Y_PIN)<100) && (data_to_send.throttle>0)) {
        data_to_send.throttle -= 1 ;
      }
    }

    data_to_send.roll = analogRead(RIGHT_JOYSTICK_X_PIN);
    data_to_send.pitch = analogRead(RIGHT_JOYSTICK_Y_PIN);
    data_to_send.throtctr = digitalRead(RIGHT_JOYSTICK_BTN_PIN);
  }else{
    data_to_send.yaw = analogRead(RIGHT_JOYSTICK_X_PIN);
    data_to_send.rth = digitalRead(RIGHT_JOYSTICK_BTN_PIN);
    if ((flags.throttle_change_disable == -1)) {
      if ((analogRead(RIGHT_JOYSTICK_Y_PIN)>1000)&& (data_to_send.throttle<100)) {
        data_to_send.throttle += 1 ;
      }
      if ((analogRead(RIGHT_JOYSTICK_Y_PIN)<100) &&(data_to_send.throttle>0)) {
        data_to_send.throttle -= 1 ;
      }
    }

    data_to_send.roll = analogRead(LEFT_JOYSTICK_X_PIN);
    data_to_send.pitch = analogRead(LEFT_JOYSTICK_Y_PIN);
    data_to_send.throtctr = digitalRead(LEFT_JOYSTICK_BTN_PIN);
  }

  if (digitalRead(SWITCH_HAND_MODE_PIN)==HIGH) {
    flags.hand_setup *= -1;
  }

  if (digitalRead(LIGHT_ENABLE_PIN)==HIGH) {
    flags.lights *= -1;
  }

  if (digitalRead(ALTITUDE_CONTROL_ENABLE_PIN)==HIGH) {
    flags.altitude_control *= -1;
  }

  if (data_to_send.throtctr == 0) {
    flags.throttle_change_disable *= -1;
  }

  int altitude;
  if (flags.altitude_control == -1) {
    altitude = 999;
  }else {
    altitude = map(analogRead(POTENTIOMETER_PIN),0,1023,0,255);
  }

  flags.engine_cut = digitalRead(ENGINE_CUT_PIN);

  //showing the readings
  Serial.print("yaw: ");
  Serial.println(data_to_send.yaw);
  Serial.print("throttle: ");
  Serial.println(data_to_send.throttle);
  Serial.print("rth: ");
  Serial.println(data_to_send.rth);

  Serial.print("roll: ");
  Serial.println(data_to_send.roll);
  Serial.print("pitch: ");
  Serial.println(data_to_send.pitch);
  Serial.print("throtctr: ");
  Serial.println(data_to_send.throtctr);
  Serial.print("throtctr_change: ");
  Serial.println(flags.throttle_change_disable);

  Serial.print("handMode: ");
  Serial.println(flags.hand_setup);
  Serial.print("lightEnable: ");
  Serial.println(flags.lights);
  Serial.print("altitudeControl: ");
  Serial.println(flags.altitude_control);

  Serial.print("altitude: ");
  Serial.println(altitude);

  Serial.print("engineCut: ");
  Serial.println(flags.engine_cut);

  char message[17];
  sprintf(message,"%03d,%03d,%02d,%02d,%02d",altitude,data_to_send.throttle,flags.lights,flags.throttle_change_disable,flags.hand_setup);
  Serial.print("message to send: ");
  Serial.println(message);
  Wire.beginTransmission(SLAVE_ADDRESS); // Start transmission to the slave
  Wire.write(message); // Send the character array
  Wire.endTransmission(); // End transmission

  delay(75);
}
