#include "DCMotor2.h"


DCMotor2::DCMotor2(int m, bool r): mMotor(m) {
	mReverse = r;
}


void DCMotor2::setSpeed(int speed) {
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



void DCMotor2::start(){
	switch(mReverse) {
		case false:
		if (mSpeed > 0) {
			mMotor.setSpeed(mSpeed);
			mMotor.run(FORWARD);
		}else {
			mMotor.setSpeed(-mSpeed);
			mMotor.run(BACKWARD);
		}
		case true:
		if (mSpeed > 0) {
			mMotor.setSpeed(mSpeed);
			mMotor.run(BACKWARD);
		}else {
			mMotor.setSpeed(-mSpeed);
			mMotor.run(FORWARD);
		}
	}
}


void DCMotor2::stop(){
	mMotor.setSpeed(0);
	mMotor.run(RELEASE);
}


void DCMotor2::brake(){
	mMotor.setSpeed(0);
	mMotor.run(FORWARD);
}