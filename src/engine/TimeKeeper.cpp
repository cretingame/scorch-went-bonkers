#include <iostream>
#include <cmath>
#include <allegro.h>

#include "TimeRoutines.h"
#include "TimeKeeper.h"

/** SYSTEM INDEPENDENT FUNCTIONS **/

TimeKeeper::TimeKeeper(double minDelta, double maxDelta, int maxFrameSkip, bool yield)
	: maxDT(maxDelta), minDT(minDelta), maxFS(maxFrameSkip), yielding(yield)
{
	frequency = getTimerFrequency();
	reset();
}

void TimeKeeper::reset()
{
	lastQPC = getTimerValue();
	framesToSkip = 0;
	ahead = 0.0;
}

double TimeKeeper::getDT()
{
	bool haveSlept = false;
	LONG_LONG now, ref, sleepTime;
	now = getTimerValue();
	
	double dt = (now - lastQPC) / (double)frequency;
	
	ref = now;
	
	if (dt < minDT)
	{
		ahead += minDT - dt;
		dt = minDT;
	}
	
	while (ahead > 0.001)
	{
		double sleptDT;
		
		if (yielding) rest(2);
		sleepTime = getTimerValue();
		if ((sleepTime - ref) > frequency / 1000)
		{
			sleptDT = (sleepTime - ref) / (double)frequency;
			ref = sleepTime;
			ahead -= sleptDT;
		}
		
		haveSlept = true;
	}
	if (haveSlept)
		now = sleepTime;
	
	if (dt > maxDT * maxFS)
		dt = maxDT * maxFS - 0.00001;
	if (dt > maxDT)
	{
		framesToSkip = (int)std::ceil(dt / maxDT);
		dt = dt / framesToSkip;
		framesToSkip--;
	}
	else
		framesToSkip = 0;
	
	lastQPC = now;
	
	return dt;
}

int TimeKeeper::getFramesToSkip()
{
	return (framesToSkip--);
}
