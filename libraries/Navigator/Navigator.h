#ifndef NAVIGATOR_H
#define NAVIGATOR_H

class NavigatorListener {
	public:
		virtual void navigationOperationTerminated(bool completed);
};

class Navigator {
public:
	virtual void moveTo(float x, float y);
	virtual void turnTo(float heading);
	virtual void go(float heading);
	virtual void stop();
};

#endif

