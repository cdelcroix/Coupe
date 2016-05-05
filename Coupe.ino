#include <AFMotor.h>
#include <Encoder.h>
#include <SharpIR.h>




AF_DCMotor leftMotor(1);
AF_DCMotor rightMotor(3);
Encoder rightEncoder(18,19);
Encoder leftEncoder(20,21);
SharpIR sharp(A8, 25, 93, 1080);


void setup1(){}

int obstaclDetected(){
  Serial.println("sajdhasjhdjsa");
  int d = sharp.distance();
  if ( d < 15 &&  d > 0){
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
    return 1;
  }
  return 0;
}



void setup() {
  Serial.begin(9600);
  Serial.println("Basic Avoidance Test:");

  // turn on motor
  leftMotor.setSpeed(200);
  rightMotor.setSpeed(200);
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}

long oldPositionLeft = -999;
long oldPositionRight = -999;

void loop() {

  while(obstaclDetected()){
  }
  leftMotor.setSpeed(200);
  rightMotor.setSpeed(200);
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);

  //Serial.print("FORWARD");
  //Serial.print("|===|");
  //Serial.println("FORWARD");
  int dis=sharp.distance();  // this returns the distance to the object you're measuring


  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
  /*while(1){
    long newPositionLeft = leftEncoder.read();
    long newPositionRight = rightEncoder.read();
    if (newPositionLeft != oldPositionLeft || newPositionRight != oldPositionRight) {
      oldPositionLeft = newPositionLeft ;
      oldPositionRight = newPositionRight ;
      Serial.print(newPositionLeft);
      Serial.print("|===|");
      Serial.println(newPositionRight);
    }
    }*/
  }
