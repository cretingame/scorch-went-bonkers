#ifndef _TIMEKEEPER_H
#define _TIMEKEEPER_H

#include <allegro.h>

/************************************************/

class TimeKeeper
{
	private:
	
	const double maxDT, minDT, maxFS;
	const bool yielding;
	
	LONG_LONG lastQPC, frequency;
	
	double ahead;
	int framesToSkip;
	
	public:
	
	TimeKeeper(double minDelta, double maxDelta, int maxFrameskip, bool yield = false);
	
	double		getDT();
	int			getFramesToSkip();

	void		reset();
};

/************************************************/

#endif
