#include <Servo.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

void setup() {
  Serial.begin(115200);

  ESC1.attach(3,1000,2000);
  ESC2.attach(5,1000,2000);
  ESC3.attach(6,1000,2000);
  ESC4.attach(9,1000,2000);

  //calibration
  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  ESC3.writeMicroseconds(2000);
  ESC4.writeMicroseconds(2000);

  delay(3000);

  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);

  delay(3000);

  Serial.println("DONE CALIBRATION");
}

void loop() {
  for (int i=0; i<=1000; i+=50) {
    ESC1.writeMicroseconds(1000 + i);
    ESC2.writeMicroseconds(1000 + i);
    ESC3.writeMicroseconds(1000 + i);
    ESC4.writeMicroseconds(1000 + i);

    Serial.print("INCREASING: ");
    Serial.println(1000+i);
  
    delay(500);
  }

  delay(2000);

  for (int i=1000; i>=0; i-=50) {
    ESC1.writeMicroseconds(1000 + i);
    ESC2.writeMicroseconds(1000 + i);
    ESC3.writeMicroseconds(1000 + i);
    ESC4.writeMicroseconds(1000 + i);

    Serial.print("DECREASING: ");
    Serial.println(1000+i);

    delay(500);
  }

  delay(3000);
}
