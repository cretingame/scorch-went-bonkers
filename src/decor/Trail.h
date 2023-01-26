#ifndef _TRAIL_H
#define _TRAIL_H

/*****************************************/

#include <deque>
#include "../Classes.h"
#include "../entity/Entity.h"

using namespace std;

/*****************************************/

struct TrailInfo
{
	double x, y;
	double time;
	
	TrailInfo(double _x, double _y, double _time)
		: x(_x), y(_y), time(_time) {};
};

/*****************************************/

class Trail
	: public Entity
{
	protected:
		
		Entity		*target;
		double		interval, persistence;
	
		deque<TrailInfo> trails;

	public:
		Trail(Entity *_target, double _interval, double _persistence);
		
		int		type() {return ETYPE_TRAIL;};
		int		renderPriority() {return 45;};
		int		stepPriority() {return 0;};
		bool		isAlive();
		
		virtual void	renderTrail(TrailInfo t) = 0;
		
		void		step(double dt);
		void		render();
};

/*****************************************/

#endif
