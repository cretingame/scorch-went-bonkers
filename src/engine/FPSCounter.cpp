#include <deque>

#include "TimeRoutines.h"
#include "FPSCounter.h"

using namespace std;

/********************************************/

FPSCounter::FPSCounter()
{
	frequency = getTimerFrequency();
	reset();
}

/********************************************/

void FPSCounter::reset()
{
	frames.clear();
}

/********************************************/

void FPSCounter::frameRendered()
{
	LONG_LONG now = getTimerValue();

	frames.push_back(now);
}

/********************************************/

int FPSCounter::getFPS()
{
	deque<LONG_LONG>::iterator it;
	LONG_LONG secondBack;
	
	secondBack = getTimerValue();
	secondBack = secondBack - frequency;
	
	it = frames.begin();
	while ( (it != frames.end()) && ((*it) < secondBack) )
		it++;
	
	if (it > frames.begin())
	{
		it--;
		frames.erase(frames.begin(), it);
	}
	
	return frames.size();
}
