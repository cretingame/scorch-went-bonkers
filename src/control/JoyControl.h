#ifndef _JOYCONTROL_H
#define _JOYCONTROL_H

#include <allegro.h>
#include "SwitchControl.h"

/***********************************************/

class JoyControl
	: public SwitchControl
{
	private:
		int joyNumber, buttonNumber;
		
	protected:
		bool isPressed() { poll_joystick(); return joy[joyNumber].button[buttonNumber].b; };
		
	public:
		JoyControl(Tank *_tank, int _joyNumber, int _buttonNumber)
			: SwitchControl(_tank), joyNumber(_joyNumber), buttonNumber(_buttonNumber) {};
};

/***********************************************/

#endif
