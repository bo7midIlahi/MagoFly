#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Servo.h>

// extern declarations (the actual objects are in main.cpp)
extern Servo ESC1;
extern Servo ESC2;
extern Servo ESC3;
extern Servo ESC4;

void setThrottle(int throttle);
void moveForward(int throttle);
void moveBackward(int throttle);
void rollLeft(int throttle);
void rollRight(int throttle);
void yawLeft(int throttle);
void yawRight(int throttle);
void KILL_ENGINES();
void returnToGround(int throttle);

#endif