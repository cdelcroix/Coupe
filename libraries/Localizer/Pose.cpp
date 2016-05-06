#include "Pose.h"

Pose::Pose(float x, float y, float heading) {
  this->x = x;
  this->y = y;
  this->heading = heading;
}

void Pose::print() const {
	Serial.print("x=");
	Serial.print(x);
	Serial.print("\ty=");
	Serial.print(y);
	Serial.print("\theading=");
	Serial.println(heading);
}

void Pose::wait() const {
}

void Pose::signal() const {
}
