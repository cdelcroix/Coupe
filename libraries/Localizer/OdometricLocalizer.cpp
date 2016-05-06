#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#endif
#include <math.h>
#include "OdometricLocalizer.h"


OdometricLocalizer::OdometricLocalizer(Encoder& leftEncoder, Encoder& rightEncoder, float wheelDiameter, float trackWidth, int countsPerRevolution):
  leftEncoder(leftEncoder), rightEncoder(rightEncoder) {
  this->distancePerCount = (M_PI * wheelDiameter)/ (float)countsPerRevolution;
  this->radiansPerCount = M_PI * (wheelDiameter / trackWidth) / countsPerRevolution;
  this->oldLeftCounts = leftEncoder.read();
  this->oldRightCounts = rightEncoder.read();
}

void OdometricLocalizer::setPose(Pose pose) {
  x = pose.x;
  y = pose.y;
  heading = pose.heading;
}

void OdometricLocalizer::setPose(float x, float y, float heading) {
  this->x = x;
  this->y = y;
  this->heading = heading;
}

void OdometricLocalizer::setPosition(float x, float y) {
  this->x = x;
  this->y = y;
}

void OdometricLocalizer::setHeading(float heading) {
  this->heading = heading;
}

Pose OdometricLocalizer::getPose() const {
  return Pose(x, y, heading);
}

void OdometricLocalizer::run() {
// periodically sample the encoder and perform calculations
// read encoders
  int leftCounts = leftEncoder.read();
  int rightCounts = -rightEncoder.read();

// calculate change in pose
  int deltaLeft = leftCounts - oldLeftCounts;
  int deltaRight = rightCounts - oldRightCounts;
  float deltaDistance = ((deltaLeft + deltaRight) / 2.0f) * distancePerCount;
  float deltaX = deltaDistance * (float)cos(heading);
  float deltaY = deltaDistance * (float)sin(heading);
  float deltaHeading = (float)(deltaRight - deltaLeft) * radiansPerCount;

// update position and heading estimates
  x += deltaX;
  y += deltaY;
  heading += deltaHeading;

// limit heading to -Pi <= heading < Pi
  if (heading > M_PI)
    heading -= 2*M_PI;
  else if (heading <= -M_PI)
    heading += 2*M_PI;

//update old position
  oldLeftCounts = leftCounts;
  oldRightCounts = rightCounts;
}
