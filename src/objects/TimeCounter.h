#ifndef _TIMECOUNTER_H
#define _TIMECOUNTER_H

/***************************************/

#include "../entity/Entity.h"

/***************************************/

class TimeCounter
	: public Entity
{
	protected:
		double		time, timeLimit, timeRescale;
	
		int			lastTimeDisplayed;
		double		buff, basicSize;
		double		animProgress;
	
	public:
		TimeCounter(double _timeLimit, double _timeRescale);
	
		void		setLimit(double limit) {timeLimit = limit;};
		double	getLimit() {return timeLimit;};
	
		int		renderPriority()		{return 5;};
		int		stepPriority()		{return 5;};
		
		double	scrollingSpeed()		{return -0.4;};
		
		void		step(double dt);
		void		render();
};

/***************************************/

#endif
