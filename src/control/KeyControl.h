#ifndef _KEYCONTROL_H
#define _KEYCONTROL_H

#include <allegro.h>
#include "SwitchControl.h"

/***********************************************/

class KeyControl
	: public SwitchControl
{
	private:
		int keyScancode;
		
	protected:
		bool isPressed() { return key[keyScancode]; };
		
	public:
		KeyControl(Tank *_tank, int scancode)
			: SwitchControl(_tank), keyScancode(scancode) {};
};

/***********************************************/

#endif
