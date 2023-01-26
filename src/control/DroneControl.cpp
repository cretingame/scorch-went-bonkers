#include <cmath>
#include <allegro.h>

#include "../control/FakeBullet.h"
#include "../entity/Entity.h"
#include "../entity/GameWorld.h"
#include "../misc/Random.h"
#include "../objects/Tank.h"

#include "DroneControl.h"

/*****************************************/

DroneControl::DroneControl(Tank *_tank, double _stupidity)
	: TankControl(_tank), stupidity(_stupidity)
{
	shotsFired = 0;
	shotReady = false;
	idealFound = false;
	target = NULL;
}

/*****************************************/

bool DroneControl::checkPress()
{
	shotsFired = 0;
	shotReady = false; 
	idealFound = false;
	target = NULL;
	return true;
}

/*****************************************/

void DroneControl::acquireTarget()
{
	target = NULL;
	if (shotReady && tank->getState() != TS_SHOOTING)
		shotReady = false;
	idealFound = false;
	shotsFired = 0;
	
	list<Entity*> l = tank->getParent()->getEntitiesOfType(ETYPE_TANK);
	list<Entity*>::iterator it;
	
	double minSquareDist = 100000000.0;
	for (it = l.begin(); it != l.end(); it++)
	{
		if (tank == (*it)) continue;
		if (!(*it)->isAlive()) continue;
		
		double diffX = tank->getX() - (*it)->getX();
		double diffY = tank->getY() - (*it)->getY();
		double squareDist = diffX * diffX + diffY * diffY;
		if (squareDist < minSquareDist)
		{
			target = (Tank*)(*it);
			minSquareDist = squareDist;
		}
	}
	
	if (target)
	{
		int direction = (target->getX() - tank->getX()) < 0 ? -1 : 1;
		idealAngle = (direction == 1) ? 25.0 : 155.0;
	}
}

/*****************************************/

PowerConstraints DroneControl::findIdealPower(double x, double angle, double eps, int bail, int depth, PowerConstraints pc)
{
	FakeBullet *fb;
	double colPoints[11];
	
	for(int i = 0; i <= 10; i++)
	{
		double power = pc.min + (pc.max - pc.min) * i / 10;
		double vx = std::cos(angle * M_PI / 180) * power * 2;
		double vy = -std::sin(angle * M_PI / 180) * power * 2;
		double shotX = tank->getX();
		double shotY = tank->getY();
		if (power != 0.0)
		{
			shotX += vx * 10 / power;
			shotY += vy * 10 / power;
		}
		
		fb = new FakeBullet(shotX, shotY, vx, vy, 3.0);
		fb->setParent(tank->getParent());
		fb->fly();
		colPoints[i] = fb->getCollisionX();
	}
	
	for(int i = 0; i < 10; i++)
	{
		if ( (colPoints[i] - x) * (colPoints[i+1] - x)  <  0)
		{
			double power1 = pc.min + (pc.max - pc.min) * i / 10;
			double power2 = pc.min + (pc.max - pc.min) * (i + 1) / 10;

			//~ PowerConstraints result = {true, power1, power2};
			//~ return result;
			
			if (std::abs(power1 - power2) <= eps)
			{
				PowerConstraints result = { (std::abs(colPoints[i] - colPoints[i+1]) < 10.0), power1, power2};
				return result;
			}
			else
			{
				PowerConstraints approx = {true, power1, power2};
				PowerConstraints result = findIdealPower(x, angle, eps, bail, depth + 1, approx);
				if (result.valid)
					return result;
			}
		}
	}
	
	PowerConstraints result = {false, 0.0, 0.0};
	return result;
}

/*****************************************/

void DroneControl::findIdealShot(double x)
{
	int direction = (x - tank->getX()) < 0 ? -1 : 1;
	
	idealPower = -1.0;
	
//	while (idealPower < 0.0)
//	{
		PowerConstraints pc = {true, 0.0, 1.1};
		pc = findIdealPower(x, idealAngle, 0.005, 4, 0, pc);
		if (pc.valid)
			idealPower = (pc.min + pc.max) / 2;
		else
		{
			idealPower = -1.0;
			idealAngle += direction * 4.0;
		}
//	}
	
	if (idealPower > 0.0)
		idealFound = true;
};

/*****************************************/

TankBehavior DroneControl::getBehavior(double dt)
{
	if (!tank) return TB_DO_NOTHING;
	if (tank->getParent()->countEntitiesOfType(ETYPE_TANK) == 1)
		return TB_DO_NOTHING;
	
	// switch targets when WE are destroyed
	if ((tank->getState() == TS_RESPAWNING) || (tank->getState() == TS_FALLING))
	{
		target = NULL;
		return TB_DO_NOTHING;
	}
	
	// switch targets when it is destroyed
	if (!target || (!target->isAlive())) 
		acquireTarget();
	if (target && ((target->getState() == TS_RESPAWNING) || (target->getState() == TS_FALLING)))
		acquireTarget();

	if (!target) return TB_DO_NOTHING;
	
	if (!idealFound)
		findIdealShot(target->getX());
	if (!idealFound)
		return TB_DO_NOTHING;
	
	if (!shotReady)
	{
		double moronFactor = (stupidity * ((stupidity * 70) - shotsFired)) / (stupidity * 70);
		if (moronFactor < stupidity / 7)
			moronFactor = stupidity / 7;

		do
		{
			double angleMod = idealAngle - 90.0;
			desiredAngle = 90.0 + randomFloat(1.0 - moronFactor, 1.0 + moronFactor) * angleMod;
			desiredPower = randomFloat(1.0 - moronFactor, 1.0 + moronFactor) * idealPower;
		}
		while (desiredAngle < 25.0 || desiredAngle > 155.0 || desiredPower < 0.04 || desiredPower > 1.1);
		
		shotReady = true;
	}
	
	int state = tank->getState();
	
	if (state == TS_IDLE)
	{
		double curAngle = tank->getAngle();
		double diff = std::abs(curAngle - desiredAngle), futureDiff = std::abs(curAngle + tank->getGunDir() * dt * 0.5 - desiredAngle);
		if ((diff < 3.0) && (diff < futureDiff))
			return TB_START_SHOOTING;
	}
	
	if (state == TS_SHOOTING)
	{
		double curPower = tank->getPower();
		if (curPower >= desiredPower)
		{
			shotReady = false;
			shotsFired++;
			return TB_SHOOT;
		}
	}
	
	return TB_DO_NOTHING;
}
