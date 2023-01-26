#ifndef _FAKEBULLET_H
#define _FAKEBULLET_H

/*********************************************/

#include "../Classes.h"
#include "../entity/Airborne.h"

/*********************************************/

class FakeBullet
	: public Airborne
{
	protected:
		double	accuracy;
		double	flightTime;
		double	colX, colY;
	
	public:
		FakeBullet(double x, double y, double vx, double vy, double accuracy);
	
		void		fly();
		double	getCollisionX() {return colX;};
		double	getCollisionY() {return colY;};
		double	getFlightTime() {return flightTime;};
};

/*********************************************/

#endif
