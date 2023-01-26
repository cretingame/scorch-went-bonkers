#include "../entity/Entity.h"
#include "../entity/GameWorld.h"
#include "../objects/Terrain.h"

#include "FakeBullet.h"

/*******************************************/

FakeBullet::FakeBullet(double _x, double _y, double _vx, double _vy, double _accuracy)
	: Airborne(_x, _y, _vx, _vy, 1.0), accuracy(_accuracy)
{
	flightTime = 0.0;
	colX = -1.0;
	colY = -1.0;
}

/*******************************************/

void FakeBullet::fly()
{
	if (!parent) return;
		
	list<Entity*> l = parent->getEntitiesOfType(ETYPE_TERRAIN);
	Terrain *terrain = (Terrain*) l.front();
	
	if (!terrain) return;
	
	while(!terrain->contains(this->x, this->y))
	{
		flightTime += accuracy;
		step(accuracy);
		if (x < 0)
		{
			x = -x;
			vx = -vx;
		}
		if (x > 800)
		{
			x = 1600 - x;
			vx = -vx;
		}
	}
	
	colX = this->x;
	colY = this->y;
}
