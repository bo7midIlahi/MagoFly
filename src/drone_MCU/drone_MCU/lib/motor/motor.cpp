#include "motor.h"

void setThrottle(int throttle){
    ESC1.writeMicroseconds(throttle);
    ESC2.writeMicroseconds(throttle);
    ESC3.writeMicroseconds(throttle);
    ESC4.writeMicroseconds(throttle);
}

void moveForward(int throttle){
    ESC1.writeMicroseconds(throttle-100);
    ESC2.writeMicroseconds(throttle-100);
    ESC3.writeMicroseconds(throttle);
    ESC4.writeMicroseconds(throttle);
}

void moveBackward(int throttle){
    ESC1.writeMicroseconds(throttle);
    ESC2.writeMicroseconds(throttle);
    ESC3.writeMicroseconds(throttle-100);
    ESC4.writeMicroseconds(throttle-100);
}

void rollLeft(int throttle){
    ESC1.writeMicroseconds(throttle-100);
    ESC2.writeMicroseconds(throttle);
    ESC3.writeMicroseconds(throttle);
    ESC4.writeMicroseconds(throttle-100);
}

void rollRight(int throttle){
    ESC1.writeMicroseconds(throttle);
    ESC2.writeMicroseconds(throttle-100);
    ESC3.writeMicroseconds(throttle-100);
    ESC4.writeMicroseconds(throttle);
}

void yawLeft(int throttle){
    ESC1.writeMicroseconds(throttle-100);
    ESC2.writeMicroseconds(throttle);
    ESC3.writeMicroseconds(throttle-100);
    ESC4.writeMicroseconds(throttle);
}

void yawRight(int throttle){
    ESC1.writeMicroseconds(throttle);
    ESC2.writeMicroseconds(throttle-100);
    ESC3.writeMicroseconds(throttle);
    ESC4.writeMicroseconds(throttle-100);
}

void KILL_ENGINES(){
    ESC1.writeMicroseconds(1000);
    ESC2.writeMicroseconds(1000);
    ESC3.writeMicroseconds(1000);
    ESC4.writeMicroseconds(1000);
    while (true)
    {
        Serial.println("ENGINE KILLED");
    }
}

void returnToGround(int throttle){
    while (throttle>1000)
    {
        throttle -= 100;
        delay(1000);
    }
}