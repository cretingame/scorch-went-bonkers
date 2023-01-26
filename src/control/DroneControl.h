#ifndef _DRONECONTROL_H
#define _DRONECONTROL_H

/****************************************/

#include "../Classes.h"
#include "../control/TankControl.h"

/****************************************/

struct PowerConstraints
{
	bool valid;
	double min, max;
};

/****************************************/

class DroneControl
	: public TankControl
{
	protected:
		
		double		stupidity;
		int			shotsFired;
	
		bool			idealFound, shotReady;
		double		idealAngle, idealPower;
		double		desiredAngle, desiredPower;

		Tank			*target;
	
		void			acquireTarget();
		PowerConstraints	findIdealPower(double x, double angle, double eps, int bail, int depth, PowerConstraints pc);
		void			findIdealShot(double x);
	
	public:
		DroneControl(Tank *_tank, double _stupidity = 0.05);
		
		bool			checkPress();
		TankBehavior	getBehavior(double dt);
};

/****************************************/

#endif
