#ifndef _FINALOPTION_H
#define _FINALOPTION_H

/************************************************/

#include "../menu/MenuOption.h"
#include "../menu/Menu.h"

/************************************************/

class FinalOption
	: public MenuOption
{
	protected:
		int optionValue;

	public:
		FinalOption(const char *_text, int _optionValue, double _marginTop = 15.0, double _marginBottom = 15.0,
			ACTION_CALLBACK _callback = NULL)
			: MenuOption(_text, _marginTop, _marginBottom, _callback), optionValue(_optionValue) {};
	
		void		action(int keyScancode) {parentMenu->setFinalChosen(optionValue); MenuOption::action(keyScancode);};
};

/************************************************/

#endif
