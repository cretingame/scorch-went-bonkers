#include <cmath>
#include <GL/gl.h>

#include "../misc/Random.h"
#include "GroundDebris.h"

/**********************************************/

GroundDebris::GroundDebris(double _x, double _y, double baseAngle, double power)
	: Airborne(_x, _y, 0.0, 0.0, 0.4)
{
	double angle = baseAngle + randomFloat(-M_PI/4.0, M_PI/4.0);
	double speed = randomFloat(power / 5, power);
	if (randomFloat(0.0, 1.0) > 0.9)
		speed *= 2.0;
	
	setVX(std::cos(angle) * speed);
	setVY(-std::sin(angle) * speed);
}

