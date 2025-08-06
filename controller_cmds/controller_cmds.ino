#include "Communication.h"

//Board adresses for the communication
#define COLTROLLER_CMD_ADRESS 10
#define COLTROLLER_LCD_ADRESS 25

static Communication comms(COLTROLLER_CMD_ADRESS);

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
  //unsigned long start =  micros();

  //setting the readings
  if (flags.hand_setup==1){
    data_to_send.yaw = analogRead(LEFT_JOYSTICK_X_PIN);
    data_to_send.rth = digitalRead(LEFT_JOYSTICK_BTN_PIN);
    if ((data_to_send.throtctr == 1)) {
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
    if ((data_to_send.throtctr == 1) && (data_to_send.throttle<=100)&&(data_to_send.throttle>=0)) {
      if ((analogRead(RIGHT_JOYSTICK_Y_PIN)>1000)) {
        data_to_send.throttle += 1 ;
      }
      if ((analogRead(RIGHT_JOYSTICK_Y_PIN)<100)) {
        data_to_send.throttle -= 1 ;
      }
    }

    data_to_send.roll = analogRead(LEFT_JOYSTICK_X_PIN);
    data_to_send.pitch = analogRead(LEFT_JOYSTICK_Y_PIN);
    data_to_send.throtctr = digitalRead(LEFT_JOYSTICK_BTN_PIN);
  }

  //int handMode = digitalRead(SWITCH_HAND_MODE_PIN);
  if (digitalRead(SWITCH_HAND_MODE_PIN)==HIGH) {
    flags.hand_setup *= -1;
  }
  //int lightEnable = digitalRead(LIGHT_ENABLE_PIN);
  if (digitalRead(LIGHT_ENABLE_PIN)==HIGH) {
    flags.lights *= -1;
  }

  //int altitudeControl = digitalRead(ALTITUDE_CONTROL_ENABLE_PIN);
  if (digitalRead(ALTITUDE_CONTROL_ENABLE_PIN)==HIGH) {
    flags.altitude_control *= -1;
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

  char screen_data[20];
  //itoa(altitude, altchar, 10);
  sprintf(screen_data, "%03d,%03d", altitude,data_to_send.throttle);
  Serial.print("sentding: ");
  Serial.println(screen_data);
/*
  String msgString = String(screen_data);
  String alt = msgString.substring(0,3);
  char alttt[4];
  alt.toCharArray(alttt, 4);
  //printf(alttt, "%s",alt);

  Serial.print("string: ");
  Serial.println(alttt);
*/
  comms.Transmit(COLTROLLER_LCD_ADRESS,altitude);

  /*for(int i=0; i<1305; i++) {
    asm volatile("nop");
  }*/

  delay(75);

  /*unsigned long finished = micros();
  Serial.print("took ");
  Serial.print(finished - start);
  Serial.println("us");*/
}
