#include <cmath>

#include "../entity/GameWorld.h"
#include "Airborne.h"

/***********************************************/

void Airborne::setParent(GameWorld *gw)
{
	Entity::setParent(gw);
	
	if (gw)
	{
		Physics p = gw->getPhysics();
		
		gravityAcc = p.gravity * weight;
		fricIndex  = 1.0 - p.airFriction;
	}
}

/***********************************************/

void Airborne::step(double dt)
{
	x += vx * dt;
	y += vy * dt + gravityAcc * 0.5 * dt * dt;
	vy += gravityAcc * dt;
	
	static double friction = std::pow(fricIndex, dt);
	vx *= friction;
	vy *= friction;
}
