#define DTI_ITERATOR deque<TrailInfo>::iterator

#include "Trail.h"

/********************************************/

Trail::Trail(Entity *_target, double _interval, double _persistence)
	: target(_target), interval(_interval * _interval), persistence(_persistence)
{
	trails.push_front(TrailInfo(target->getX(), target->getY(), 0.0));
}

/********************************************/

bool Trail::isAlive()
{
	return ((!trails.empty()) || target);
}

/********************************************/

void Trail::step(double dt)
{
	DTI_ITERATOR it;
	
	if (target && (!target->isAlive()))
	{
		target = NULL;
	};
	
	for (it = trails.begin(); it != trails.end(); it++)
	{
		(*it).time += dt;
	}
	
	while ( (!trails.empty())  &&  ((trails.back()).time >= persistence) )
		trails.pop_back();
	
	if (target)
	{
		bool addNew = false;
		if (trails.empty())
			addNew = true;
		else
		{
			TrailInfo last = trails.front();
			double diffSquared = (last.x - target->getX()) * (last.x - target->getX()) + (last.y - target->getY()) * (last.y - target->getY());
			if (diffSquared > interval)
				addNew = true;
		}
		
		if (addNew)
			trails.push_front(TrailInfo(target->getX(), target->getY(), 0.0));
	}
}

/*************************************************/

void Trail::render()
{
	DTI_ITERATOR it;

	for (it = trails.begin(); it != trails.end(); it++)
		renderTrail(*it);
}
