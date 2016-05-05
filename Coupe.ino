#include <AFMotor.h>
#include <Encoder.h>
#include <SharpIR.h>
#include <Servo.h> 

Servo funny;
AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(1);
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

unsigned long begin;

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Avoidance Test:");

  // turn on motor
  leftMotor.setSpeed(210);
  rightMotor.setSpeed(180);
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  funny.attach(10);
  funny.write(45);
  begin = millis();
}

long oldPositionLeft = -999;
long oldPositionRight = -999;


bool finish = false;
bool quit = false;
void loop() {
  if(!quit){
    if(finish){
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      delay(2000);
      funny.write(25);
      delay(1000);
      funny.write(45);
      quit = true;
    } else {
      while(millis()-begin < 2000 && obstaclDetected()){}
      leftMotor.setSpeed(210);
      rightMotor.setSpeed(180);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      if(millis()-begin > 2000){
        finish = true;
      }
      
    }
  }
}
