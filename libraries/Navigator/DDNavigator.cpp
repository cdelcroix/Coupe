#include <math.h>
#include "include/Navigator/DDNavigator.h"

DDNavigator::DDNavigator(Motor& leftMotor, Motor& rightMotor, Localizer& localizer, 
	int driveSpeed, int rotateSpeed, float gain, float goToThreshold, float rotateThreshold) : 
	mLeftMotor(leftMotor),mRightMotor(rightMotor),mLocalizer(localizer) {

	mDriveSpeed = driveSpeed;
	mRotateSpeed = rotateSpeed;
	mGain = gain;
	mGoToThreshold = goToThreshold;
	mRotateThreshold = rotateThreshold;
	mState = DONE;
}

void DDNavigator::run() {
	while (true) {
		switch (mState) {
			case MOVETO:
			goToPoint();
			break;
			case GO:
			goHeading();
			break;
			case ROTATE:
			doRotate();
			break;
			default: // stopped
			break;
		}
	}
}


float DDNavigator::normalizeAngle(float angle) {
	while (angle < -PI)
		angle += TWO_PI;
	while (angle > PI)
		angle -= TWO_PI;
	return angle;
}

void DDNavigator::moveTo(float x, float y) {
	mDestinationX = x;
	mDestinationY = y;
	mState = MOVETO;
}

void DDNavigator::turnTo(float heading) {
	mTargetHeading = normalizeAngle(heading);
	mState = ROTATE;
}


void DDNavigator::doRotate() {
	Pose pose = mLocalizer.getPose();
	float error = mTargetHeading - pose.heading;
// choose the direction of rotation that results
// in the smallest angle
	if (error > PI)
		error -= TWO_PI;
	else if (error < -PI)
		error += TWO_PI;
	float absError = (error >= 0.0f) ? error : -error;
	if (absError < mRotateThreshold) {
		mLeftMotor.setSpeed(0);
		mRightMotor.setSpeed(0);
		mState = DONE;
	}
	else if (error > 0.0f) {
		mLeftMotor.setSpeed(-mRotateSpeed);
		mRightMotor.setSpeed(mRotateSpeed);
	}
	else {
		mLeftMotor.setSpeed(mRotateSpeed);
		mRightMotor.setSpeed(-mRotateSpeed);
	}
}

void DDNavigator::goToPoint() {
	Pose pose = mLocalizer.getPose();
	float xError = mDestinationX - pose.x;
	float yError = mDestinationY - pose.y;

	float absXError = (xError > 0.0f) ? xError : -xError;
	float absYError = (yError > 0.0f) ? yError : -yError;
	if ((absXError + absYError) < mGoToThreshold) {
// stop
		mLeftMotor.setSpeed(0);
		mRightMotor.setSpeed(0);
		mState = DONE;
	}
	else {
// adjust heading and go that way
		mTargetHeading = (float)atan2(yError, xError);
		goHeading();
	}
}

void DDNavigator::goHeading() {
	Pose pose = mLocalizer.getPose();
	float error = mTargetHeading - pose.heading;
	if (error > PI)
		error -= TWO_PI;
	else if (error < -PI)
		error += TWO_PI;

	int differential = (int)(mGain * error + 0.5f);

	mLeftMotor.setSpeed(mDriveSpeed - differential);
	mRightMotor.setSpeed(mDriveSpeed + differential);
}

void DDNavigator::go(float heading) {
	mTargetHeading = normalizeAngle(heading);
	mState = GO;
}

void DDNavigator::stop() {
	mLeftMotor.setSpeed(0);
	mRightMotor.setSpeed(0);
	mState = DONE;
}