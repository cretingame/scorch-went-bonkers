#include <allegro.h>

#include "../misc/Settings.h"

#include "SwitchControl.h"


const double tapTime = 30.0;

SwitchControl::SwitchControl(Tank *_tank)
	: TankControl(_tank)
{
	previouslyPressed = false;
}

/***********************************************/

bool SwitchControl::checkPress()
{
	return isPressed();
}

/***********************************************/

TankBehavior SwitchControl::getBehavior(double dt)
{
	bool pressed = isPressed();
	
	if (pressed)
	{
		pressedTime += dt;
	}
	
	if (pressed && (!previouslyPressed))
	{
		pressedTime = 0.0;
		previouslyPressed = true;
		if (Settings::setting[ACC_SLOW_SHOOTING])
			return TB_SLOW_SHOOTING;
		else
			return TB_START_SHOOTING;
	}
	
	if (pressed && previouslyPressed && Settings::setting[ACC_SLOW_SHOOTING] && (pressedTime > tapTime * (1.0 + 0.5 * Settings::setting[ACC_TAP_SPEED])))
	{
		return TB_START_SHOOTING;
	}
	
	if (previouslyPressed && (!pressed))
	{
		previouslyPressed = false;
		if (pressedTime > tapTime * (1.0 + 0.5 * Settings::setting[ACC_TAP_SPEED]))
			return TB_SHOOT;
		else
			return TB_SHIELD;
	}
	
	return TB_DO_NOTHING;
}
