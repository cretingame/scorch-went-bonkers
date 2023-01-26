#ifndef _GROUNDDEBRIS_H
#define _GROUNDDEBRIS_H

/************************************************/

#include "../entity/Airborne.h"
#include "../engine/GLColor.h"

/************************************************/

class GroundDebris
	: public Airborne
{	
	public:
		GroundDebris(double _x, double _y, double baseAngle, double power);
	
		bool		isAlive() {return (x > 0  &&  x < 800  &&  y < 600);};
};

/*************************************************/

#endif
