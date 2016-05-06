#ifndef _POSE_H
#define _POSE_H

#include <Arduino.h>

class Pose {
public:
  float y;
  float x;
  float heading;
  Pose(float x, float y, float heading);
  void print() const;
  void wait() const;
  void signal() const;
};

#endif
