/**
 * DCMotor - A simple library to control DC motors with L298N H-Bridge
 * MADE IN eirbot.fr
 */


#ifndef DCMOTOR_H
#define DCMOTOR_H


#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#endif
 
#include <Arduino.h>
#include "Motor.h"

class DCMotor : public Motor {
private:
	static const int MAX_SPEED = 255;
	int mSpeed;
	bool mReverse;
	int mPin1; // Reverse pin
	int mPin2; // Speed pin

public:
	/**
	* Full Constructor
	*/
	DCMotor(byte pin1, byte pin2, bool = false);

	/**
	* Set PWM Speed
	*/
	void setSpeed(int) override;


	/**
	* Start the motor
	*/
	void start() ;
	void startCharles();
		
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