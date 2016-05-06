#include <Thread.h>
#include <ThreadController.h>
#include <AFMotor.h>
#include <Encoder.h>
#include <SharpIR.h>
#include <Servo.h> 

#define TIME_DOORS 91000
#define TIME_FUNNY 91000

ThreadController controll = ThreadController();
Thread PIDThread = Thread();

unsigned long lastMilli = 0;
int speedL_req = 80;
int speedL_act = 0; //actual value
int speedR_req = 80;
int speedR_act = 0; //actual value
int PWML_val = 0;
int PWMR_val = 0;
volatile long countL = 0; // revolution counter
volatile int count1L = 0; // revolution counter for position
volatile long countR = 0; // revolution counter
volatile int count1R = 0; // revolution counter for position
float Kp = 0.4;          //setting Kp  
float Kd = 1;            //setting Kd

Servo funny;
AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(1);
Encoder rightEncoder(18,19);
Encoder leftEncoder(20,21);
SharpIR sharp(A8, 25, 93, 1080);


int obstaclDetected(){
  return 0;
  int d = sharp.distance();
  Serial.println(d);
  if ( d < 25 &&  d > 0){
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
    return 1;
  }
  return 0;
}

unsigned long begin;


void getMotorData(unsigned long last)  {                                       // calculate speed
  static long countAntL = 0;                                                    // last count
  static long countAntR = 0;
  //Calculating the speed using encoder count
  //Serial.println(millis()-last);
  //speedL_act = ((countL - countAntL)*(60*(1000/(millis()-last))))/(30);
  //speedR_act = ((countR - countAntR)*(60*(1000/(millis()-last))))/(30);
  speedL_act = (countL - countAntL)*(255.0/70);
  speedR_act = (countR - countAntR)*(255.0/70);
  Serial.println(((countL - countAntL))); 
  countAntL = countL;                                           //setting count value to last count
  countAntR = countR;
}
int updatePid(int command, int targetValue, int currentValue)   {      // compute PWM value
  float pidTerm = 0;                                                           // PID correction
  int error=0;                                  
  static int last_error=0;                             
  error = abs(targetValue) - abs(currentValue); 
  pidTerm = (Kp * error) + (Kd * (error - last_error));                            
  last_error = error;
  return constrain(command + int(pidTerm), 0, 255);
}


void PIDStep(){
  static unsigned long lastTimeCall = 0;
  
  getMotorData(lastTimeCall);
  PWML_val= updatePid(PWML_val, speedL_req, speedL_act);
  PWMR_val= updatePid(PWMR_val, speedR_req, speedR_act);
  Serial.print(speedL_act);
  Serial.print(" === ");
  Serial.println(speedR_act);

  leftMotor.setSpeed(PWML_val);
  rightMotor.setSpeed(PWMR_val);
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  lastTimeCall = millis();
}

void setup() {
  Serial.begin(9600);
  // turn on motor
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  funny.attach(10);
  funny.write(20);
  begin = millis();
  
  speedL_req = 100;
  speedR_req = 100;
  
  PIDThread.onRun(PIDStep);
  PIDThread.setInterval(10);
  
  controll.add(&PIDThread);
}

bool finish = false;
bool quit = false;
void loop() {
  controll.run();
  //funny.write(20);
  //return;
  countL = leftEncoder.read();
  countR = -rightEncoder.read();

  lastMilli = millis();
  if(!quit){
    if(lastMilli - begin > TIME_FUNNY){
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      delay(10);
      funny.write(50);
      //delay(1000);
      //funny.write(20);
      quit = true;
    } else if (lastMilli -begin > TIME_DOORS) {
      //TODO
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
    
    } else {
      while(millis()-begin < TIME_DOORS && obstaclDetected()){}
 
    }
    
  }
  
}
