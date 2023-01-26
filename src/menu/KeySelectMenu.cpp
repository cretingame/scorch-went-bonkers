
#include <allegro.h>

#include "../control/JoyControl.h"
#include "../control/KeyControl.h"
#include "../control/MouseControl.h"
#include "../misc/KeyReader.h"
#include "../sound/SoundSystem.h"

#include "KeySelectMenu.h"

/*********************************************/

KeySelectMenu::KeySelectMenu(double _x, double _y)
	: Menu(_x, _y)
{
	control = NULL;
}

/*********************************************/

void KeySelectMenu::checkKeys()
{
	static const int SPECIAL_KEY[] = {KEY_LSHIFT, KEY_RSHIFT, KEY_LCONTROL, KEY_RCONTROL, KEY_ALT, KEY_ALTGR};
	static const int SPECIAL_KEYS_COUNT = 6;
	
	if (control) return;
		
	for(KEY_ITERATOR k = KeyReader::scancodes.begin(); k != KeyReader::scancodes.end(); k++)
	{
		int scancode = (*k);
		switch(scancode)
		{
			case KEY_UP:
			case KEY_DOWN:
			case KEY_C:
			case KEY_P:
			case KEY_ESC:
				break;
			default:
				control = new KeyControl(NULL, scancode);
				soundSys->playSound(SND_MENU_SELECT, 128, 0.5, 1.0);
				break;
		}
	}
	
	for (int keyNr = 0; keyNr < SPECIAL_KEYS_COUNT; keyNr++)
		if (key[SPECIAL_KEY[keyNr]])
			control = new KeyControl(NULL, SPECIAL_KEY[keyNr]);
		
	if (mouse_b > 0)
	{
		control = new MouseControl(NULL, mouse_b);
		soundSys->playSound(SND_MENU_SELECT, 128, 0.5, 1.0);
	}
	
	poll_joystick();
	for (int joyNr = 0; joyNr < num_joysticks; joyNr++)
		for (int buttNr = 0; buttNr < joy[joyNr].num_buttons; buttNr++)
			if (joy[joyNr].button[buttNr].b)
			{
				control = new JoyControl(NULL, joyNr, buttNr);
				soundSys->playSound(SND_MENU_SELECT, 128, 0.5, 1.0);
			}
}
