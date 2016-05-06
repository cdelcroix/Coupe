#include "DCMotor.h"

DCMotor::DCMotor(byte pin1, byte pin2, bool r) {
	mPin1 = pin1;
	mPin2 = pin2;
	pinMode(mPin1, OUTPUT);
	pinMode(mPin2, OUTPUT);
	mReverse = r;
}

void DCMotor::setSpeed(int speed) {
	if (speed > MAX_SPEED){
		mSpeed = MAX_SPEED;
	}
	else if (speed < -MAX_SPEED){
		mSpeed = -MAX_SPEED;
	}
	if (mSpeed == 0)
		stop();
	else
		start();
}

void DCMotor::startCharles(){
	switch(mReverse) {
		case false:
		if (mSpeed > 0) {
			analogWrite(mPin2, 0);
			analogWrite(mPin1, mSpeed);
		}else {
			analogWrite(mPin1, 0);
			analogWrite(mPin2, -mSpeed);
		}
		case true:
		if (mSpeed > 0) {
			analogWrite(mPin2, 0);
			analogWrite(mPin1, mSpeed);
		}else {
			analogWrite(mPin1, 0);
			analogWrite(mPin2, -mSpeed);
		}
	}
}

void DCMotor::start() {
	switch(mReverse) {
		case false:
		if (mSpeed > 0) {
			digitalWrite(mPin1, LOW);
		}else {
			digitalWrite(mPin1, HIGH);
		}
		case true:
		if (mSpeed > 0) {
			digitalWrite(mPin1, HIGH);
		}else {
			digitalWrite(mPin1, LOW);
		}
	}
	analogWrite(mPin2, mSpeed);
}

void DCMotor::stop(){
	analogWrite(mPin2, 0);
}

void DCMotor::brake(){
	digitalWrite(mPin2, LOW);
	digitalWrite(mPin1, LOW);
}