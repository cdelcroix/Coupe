// #include <Thread.h>
// #include <ThreadController.h>

// // ThreadController that will controll all threads
// ThreadController controll = ThreadController();

// //My Thread (as a pointer)
// Thread* myThread = new Thread();
// //His Thread (not pointer)
// Thread hisThread = Thread();

// // callback for myThread
// void niceCallback(){
// 	Serial.print("COOL! I'm running on: ");
// 	Serial.println(millis());
// }

// // callback for hisThread
// void boringCallback(){
// 	Serial.println("BORING...");
// }

// void setup(){
// 	Serial.begin(9600);

// 	// Configure myThread
// 	myThread->onRun(niceCallback);
// 	myThread->setInterval(500);

// 	// Configure myThread
// 	hisThread.onRun(boringCallback);
// 	hisThread.setInterval(250);

// 	// Adds both threads to the controller
// 	controll.add(myThread);
// 	controll.add(&hisThread); // & to pass the pointer to it
// }

// void loop(){
// 	// run ThreadController
// 	// this will check every thread inside ThreadController,
// 	// if it should run. If yes, he will run it;
// 	controll.run();

// 	// Rest of code
// 	float h = 3.1415;
// 	h/=2;
// }



#include <AFMotor.h>
#include <Encoder.h>
#include <SharpIR.h>
#include <SimpleLocalizer.h>
#include <Thread.h>
#include <ThreadController.h>

AF_DCMotor leftMotor(3);
AF_DCMotor rightMotor(1);
Encoder rightEncoder(18,19);
Encoder leftEncoder(20,21);
SharpIR sharp(A8, 25, 93, 1080);
SimpleLocalizer localizer(leftEncoder, rightEncoder, 6.2, 17.7, 960);

ThreadController controll = ThreadController();
Thread updatePoseThread = Thread();

void updatePose(){
	localizer.run();
	localizer.getPose().print();
}



void setup() {
	Serial.begin(9600);
	updatePoseThread.onRun(updatePose);
	updatePoseThread.setInterval(250);
	controll.add(&updatePoseThread);
}

void loop() {
  controll.run();
}