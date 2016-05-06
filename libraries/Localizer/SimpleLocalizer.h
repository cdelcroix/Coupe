#ifndef _SIMPLEL_OCALIZER_H
#define _SIMPLEL_OCALIZER_H

#include <Encoder.h>
#include "Pose.h"
#include "Localizer.h"

class SimpleLocalizer: public Localizer {

private:
  Encoder& leftEncoder;
  Encoder& rightEncoder;
  float distancePerCount;
  float radiansPerCount;
  float x = 0.0f;
  float y = 0.0f;
  float heading = 0.0f;
  int oldLeftCounts;
  int oldRightCounts;

public:
  SimpleLocalizer(Encoder& leftEncoder, Encoder& rightEncoder, float wheelDiameter, float trackWidth, int countsPerRevolution);

  void setPose(Pose pose) override;

  void setPose(float x, float y, float heading) override ;

  void setPosition(float x, float y) override;

  void setHeading(float heading) override;

  Pose getPose() const override;

  void run() override;
};

#endif
