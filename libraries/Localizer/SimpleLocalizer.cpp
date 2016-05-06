#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#endif
#include <math.h>
#include "SimpleLocalizer.h"

SimpleLocalizer::SimpleLocalizer(Encoder& leftEncoder, Encoder& rightEncoder, float wheelDiameter, float trackWidth, int countsPerRevolution):
  leftEncoder(leftEncoder), rightEncoder(rightEncoder) {
  this->distancePerCount = (M_PI * wheelDiameter)/ (1.0*countsPerRevolution);
  this->radiansPerCount = M_PI * (wheelDiameter / trackWidth) / countsPerRevolution;
  this->oldLeftCounts = leftEncoder.read();
  this->oldRightCounts = rightEncoder.read();
}

void SimpleLocalizer::setPose(Pose pose) {
  x = pose.x;
  y = pose.y;
  heading = pose.heading;
}

void SimpleLocalizer::setPose(float x, float y, float heading) {
  this->x = x;
  this->y = y;
  this->heading = heading;
}

void SimpleLocalizer::setPosition(float x, float y) {
  this->x = x;
  this->y = y;
}

void SimpleLocalizer::setHeading(float heading) {
  this->heading = heading;
}

Pose SimpleLocalizer::getPose() const {
  return Pose(x, y, heading);
}

void SimpleLocalizer::run() {
// periodically sample the encoder and perform calculations
// read encoders
  long leftCounts = leftEncoder.read();
  long rightCounts = -rightEncoder.read();
// calculate new pose
  float distance = ((leftCounts + rightCounts)*1.0f / 2.0f) * distancePerCount;
  heading = (1.0f*(rightCounts - leftCounts) * radiansPerCount);
  y = distance * sin(heading);
  x = distance * cos(heading);
}
