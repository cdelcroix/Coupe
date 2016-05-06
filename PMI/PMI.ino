#include <AFMotor.h>
#include <Encoder.h>
#include <SharpIR.h>
#include <SimpleLocalizer.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Servo.h> 

/*#define SPEED 40
#define TIME_BACK 4000
#define TIME_TURN_RIGHT 8000
#define TIME_FORWARD 10000
#define TIME_TURN_LEFT 12000
#define TIME_FORWARD2 14000
#define TIME_WAIT 18000
#define TIME_FUNNY 91000*/

#define SPEED 40
bool step_back = false;
bool step_turn = false;
bool step_forward = false;
bool step_turn2 = false;
bool step_forward2 = false;
bool step_wait = false;
bool step_funny = false;

#define TIME_FUNNY 91000

bool forwardL;
bool forwardR;
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
float Kp = 0.2;          //setting Kp
float Kd = 0.1;            //setting Kd

Servo funny;
AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(1);
Encoder rightEncoder(18,19);
Encoder leftEncoder(20,21);
SharpIR sharp(A8, 25, 93, 1080);
ThreadController controll = ThreadController();
Thread PIDThread = Thread();
Thread updatePoseThread = Thread();
SimpleLocalizer localizer(leftEncoder, rightEncoder, 6.2, 17.7, 960);

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


void getMotorData(unsigned long last)  {
    static long countAntL = 0;
    static long countAntR = 0;
    speedL_act = (countL - countAntL)*(255.0/70);
    speedR_act = (countR - countAntR)*(255.0/70);
    Serial.println(((countL - countAntL)));
    countAntL = countL;
    countAntR = countR;
}
int updatePid(int command, int targetValue, int currentValue)   {
    float pidTerm = 0;
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
    leftMotor.run(forwardL ? FORWARD : BACKWARD);
    rightMotor.run(forwardR ? FORWARD : BACKWARD);
    lastTimeCall = millis();
}

void updatePose(){
    localizer.run();
    localizer.getPose().print();
}

void setup() {
    Serial.begin(9600);
    // turn on motor
    speedL_req = 100;
    speedR_req = 100;
    forwardL = true;
    forwardR = true;

    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    leftMotor.run(forwardL ? FORWARD : BACKWARD);
    rightMotor.run(forwardR ? FORWARD : BACKWARD);
    funny.attach(10);
    funny.write(20);
    begin = millis();

    // PIDThread.onRun(PIDStep);
    // PIDThread.setInterval(1);

    updatePoseThread.onRun(updatePose);
    updatePoseThread.setInterval(250);
    controll.add(&updatePoseThread);

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

    Serial.print(countL);
    Serial.print(" === ");
    Serial.println(countR);

    PIDStep();

    lastMilli = millis();
    if(!quit){
        if(step_funny){
            speedL_req = 0;
            speedR_req = 0;
            delay(10);
            funny.write(50);
            //delay(1000);
            //funny.write(20);
            quit = true;
        } else if (step_funny) {
            if(lastMilli - begin > TIME_FUNNY)
                step_funny = true;
            speedL_req = 0;
            speedR_req = 0;
            forwardL = true;
            forwardR = true;

        } else if (step_forward2) {
            step_wait = true;

        } else if (step_turn2) {
            step_forward2 = true;

        } else if (step_forward) {
            step_turn2 = true;

        } else if (step_turn) {
            //TODO
            if(countL > 1360 && countR > 0)
                step_forward = true;
            speedL_req = SPEED;
            speedR_req = 0;
            forwardL = true;
            forwardR = true;

        } else if (step_back) {
            //TODO
            if(countL < 0 && countR < 0)
                step_turn = true;
            speedL_req = SPEED;
            speedR_req = SPEED;
            forwardL = false;
            forwardR = false;

        } else {
            if(countL > 3260 && countR > 3300)
                step_back = true;
            speedL_req = SPEED;
            speedR_req = SPEED;
            forwardL = true;
            forwardR = true;

        }

    }

}
