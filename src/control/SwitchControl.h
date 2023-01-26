#ifndef _SWITCHCONTROL_H
#define _SWITCHCONTROL_H

#include "TankControl.h"
#include "../Classes.h"

/**********************************************/

class SwitchControl
	: public TankControl
{
	private:
	
		bool		previouslyPressed;
		double	pressedTime;
	
	protected:
		virtual bool isPressed() = 0;
	
	public:
		
		SwitchControl(Tank *_tank);
	
		TankBehavior	getBehavior(double dt);
		bool			checkPress();
};

/**********************************************/

#endif
