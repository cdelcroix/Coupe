#ifndef DCMOTOR2_H
#define DCMOTOR2_H

#include <AFMotor.h>

class DCMotor2 {
private:
	static const int MAX_SPEED = 255;
	int mSpeed;
	bool mReverse;
	AF_DCMotor mMotor;

public:
	/**
	* Full Constructor
	*/
	DCMotor2(int m, bool r);

	/**
	* Set PWM Speed
	*/
	void setSpeed(int speed) ;


	/**
	* Start the motor
	*/
	void start();

	/**
	* Stop the motor (Freespin)
	*/
	void stop();
	/**
	* Stop the motor (No Freespin)
	*/
	void brake();

};

#endif
