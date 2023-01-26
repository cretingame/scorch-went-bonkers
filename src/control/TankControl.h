#ifndef _TANKCONTROL_H
#define _TANKCONTROL_H

/***********************************************/

#include "../Classes.h"

/***********************************************/

enum TankBehavior {TB_DO_NOTHING, TB_START_SHOOTING, TB_SHOOT, TB_SHIELD, TB_SLOW_SHOOTING};

class TankControl
{
	protected:
		Tank		*tank;
	
	public:

		TankControl(Tank *_tank = NULL) { tank = _tank; };
		
		virtual TankBehavior getBehavior(double dt) = 0;
		
		virtual bool	checkPress() = 0;
		
		void setControlled(Tank *_tank) {tank = _tank;};
};

/***********************************************/

#endif
