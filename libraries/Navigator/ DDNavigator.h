#ifndef DDNAVIGATOR_H
#define DDNAVIGATOR_H 

#include "Navigator.h"

class DDNavigator : Navigator {
private:
	static const float PI = 3.14159f;
	static const float TWO_PI = PI * 2.0f;
	static const float PI_OVER_2 = PI / 2.0f;
	static const int STOP = 0;
	static const int GO = 1;
	static const int MOVE_TO = 2;
	static const int ROTATE = 3;
	Motor& mLeftMotor;
	Motor& mRightMotor;
	Localizer& mLocalizer;
	NavigatorListener& mListener;
	int mDrivePower;
	int mRotatePower;
	int mPeriod;
	int mState;
	float mDestinationX;
	float mDestinationY;
	float mTargetHeading;
	float mGain;
	float mGoToThreshold;
	float mRotateThreshold;

public:
	DifferentialDriveNavigator(Motor leftMotor, Motor rightMotor, Localizer localizer, int drivePower, int rotatePower, float gain, float goToThreshold, float rotateThreshold, int threadPriority, int period) {
		mLeftMotor = leftMotor;
		mRightMotor = rightMotor;
		mLocalizer = localizer;
		mDrivePower = drivePower;
		mRotatePower = rotatePower;
		mGain = gain;
		mGoToThreshold = goToThreshold;
		mRotateThreshold = rotateThreshold;
		mPeriod = period;
		mState = STOP;
		mListener = null;
		setPriority(threadPriority);
		setDaemon(true);
		start();
	}

	void run() {
		while (true) {
			switch (mState) {
				case MOVE_TO:
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
			Thread.sleep(mPeriod);
		}
	}

	void updateListener(boolean completed, NavigatorListener newListener) {
		if (mListener != null)
			mListener.navigationOperationTerminated(completed);
		mListener = newListener;
	}
	
	float normalizeAngle(float angle) {
		while (angle < -PI)
			angle += TWO_PI;
		while (angle > PI)
			angle -= TWO_PI;
		return angle;
	}

	synchronized void moveTo(float x, float y, boolean wait, NavigatorListener listener) {
		updateListener(false, listener);
		mDestinationX = x;
		mDestinationY = y;
		mState = MOVE_TO;
		if (wait)
			wait();
	}

	synchronized void doRotate() {
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
			mLeftMotor.setPower(Motor.STOP);
			mRightMotor.setPower(Motor.STOP);
			mState = STOP;

 // notify listener the operation is complete
			updateListener(true, null);

// signal waiting thread we are at the destination
			notify();
		}
		else if (error > 0.0f) {
			mLeftMotor.setPower(-mRotatePower);
			mRightMotor.setPower(mRotatePower);
		}
		else {
			mLeftMotor.setPower(mRotatePower);
			mRightMotor.setPower(-mRotatePower);
		}
	}

	synchronized void goToPoint() {
		Pose pose = mLocalizer.getPose();
		float xError = mDestinationX - pose.x;
		float yError = mDestinationY - pose.y;

		float absXError = (xError > 0.0f) ? xError : -xError;
		float absYError = (yError > 0.0f) ? yError : -yError;
		if ((absXError + absYError) < mGoToThreshold) {
 // stop
			mLeftMotor.setPower(Motor.STOP);
			mRightMotor.setPower(Motor.STOP);
			mState = STOP;
 // notify listener the operation is complete
			updateListener(true, null);

 // signal waiting thread we are at the destination
			notify();
		}
		else {
 // adjust heading and go that way
			mTargetHeading = (float)Math.atan2(yError, xError);
			goHeading();
		}
	}

	synchronized void goHeading() {
		Pose pose = mLocalizer.getPose();
		float error = mTargetHeading - pose.heading;
		if (error > PI)
			error -= TWO_PI;
		else if (error < -PI)
			error += TWO_PI;

		int differential = (int)(mGain * error + 0.5f);

		mLeftMotor.setPower(mDrivePower - differential);
		mRightMotor.setPower(mDrivePower + differential);
	}

	

	void moveTo(float x, float y, boolean wait) {
		moveTo(x, y, wait, null);
	}

	void moveTo(float x, float y, NavigatorListener listener) {
		moveTo(x, y, false, listener);
	}

	synchronized void turnTo(float heading, boolean wait, NavigatorListener listener) {
		updateListener(false, listener);
		mTargetHeading = normalizeAngle(heading);
		mState = ROTATE;
		if (wait)
			wait();
	}

	void turnTo(float heading, boolean wait) {
		turnTo(heading, wait, null);
	}
	void turnTo(float heading, NavigatorListener listener) {
		turnTo(heading, false, listener);
	}

	synchronized void go(float heading) {
		mTargetHeading = normalizeAngle(heading);
		mState = GO;
		updateListener(false, null);
	}

	synchronized void stop() {
		mLeftMotor.setPower(Motor.STOP);
		mRightMotor.setPower(Motor.STOP);
		mState = STOP;
		updateListener(false, null);
	}
};