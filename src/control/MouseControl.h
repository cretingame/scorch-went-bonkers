#ifndef _MOUSECONTROL_H
#define _MOUSECONTROL_H

#include "SwitchControl.h"

/***********************************************/

class MouseControl
	: public SwitchControl
{
	protected:
		int		button;
	
		bool isPressed() { return (mouse_b & button); };
		
	public:
		MouseControl(Tank *_tank, int _button)
			: SwitchControl(_tank), button(_button) {};
};

/***********************************************/

#endif
