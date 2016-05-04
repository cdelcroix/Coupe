#include <AFMotor.h>
#include <DCMotor2.h>
#include <Encoder.h>

DCMotor2 leftMotor(1);
DCMotor2 rightMotor(3);
Encoder rightEncoder(18,19);
Encoder leftEncoder(20,21);

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Motor/Encoder Test:");
}

long oldPositionLeft = -999;
long oldPositionRight = -999;

void loop() {
  leftMotor.setSpeed(200);
  rightMotor.setSpeed(200);
  Serial.print("FORWARD");
  Serial.print("|===|");
  Serial.println("FORWARD");
  while(1){
    long newPositionLeft = leftEncoder.read();
    long newPositionRight = rightEncoder.read();
    if (newPositionLeft != oldPositionLeft || newPositionRight != oldPositionRight) {
      oldPositionLeft = newPositionLeft ;
      oldPositionRight = newPositionRight ;
      Serial.print(newPositionLeft);
      Serial.print("|===|");
      Serial.println(newPositionRight);
    }
  }
}


