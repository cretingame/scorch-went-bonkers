#ifndef _ASCENDEROPTION_H
#define _ASCENDEROPTION_H

/**************************************************/

#include "../Classes.h"
#include "../menu/MenuOption.h"
#include "../menu/Menu.h"

/**************************************************/

class AscenderOption
	: public MenuOption
{
	public:
		AscenderOption(const char *_text, double _marginTop = 15.0, double _marginBottom = 15.0, ACTION_CALLBACK _callback = NULL)
			: MenuOption(_text, _marginTop, _marginBottom, _callback) {};
	
		void		action(int keyScancode);
};

/**************************************************/

#endif
