#ifndef LOCALIZER_H
#define LOCALIZER_H

#include "Pose.h"

class Localizer {
public:
	virtual void setPose(Pose pose);

	virtual void setPose(float x, float y, float heading) ;

	virtual void setPosition(float x, float y);

	virtual void setHeading(float heading);

	virtual Pose getPose() const;

	virtual void run();
};

#endif