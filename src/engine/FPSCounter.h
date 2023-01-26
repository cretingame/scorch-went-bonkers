#ifndef _FPSCOUNTER_H
#define _FPSCOUNTER_H

/********************************/

#include <deque>

/********************************/

class FPSCounter
{
	private:
		
	LONG_LONG frequency, start;
	std::deque<LONG_LONG> frames;
	
	public:
	
	FPSCounter();
	
	void		reset();
	void		frameRendered();
	
	int		getFPS();
};

/********************************/

#endif
