ifndef _TRAIL_H
#define _TRAIL_H

/*****************************************/

#include <deque>
#include "../Classes.h"

using namespace std;

/*****************************************/

struct TrailInfo
{
	double x, y;
	double size;
};

/*****************************************/

class Trail
	: public Entity
{
	protected:
		
		Bullet		*bullet;

		deque<SmokeInfo> smokes;

	public:
		SmokeTrail(Bullet *_bullet, double _interval);
		
		void		renderPriority() {return 45;};
		void		stepPriority() {return 45;};
		
		void		step(double dt);
		void		render();
};

/*****************************************/

#endif