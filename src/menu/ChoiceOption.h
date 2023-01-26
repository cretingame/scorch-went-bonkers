#ifndef _CHOICEOPTION_H
#define _CHOICEOPTION_H

/******************************************************/

#include "../Classes.h"
#include "../menu/AscenderOption.h"

/******************************************************/

class ChoiceOption
	: public AscenderOption
{
	protected:
		int *target;
		int value;
	
	public:
		ChoiceOption(const char *_text, int *_target, int _value, double _marginTop = 15.0, double _marginBottom = 15.0, ACTION_CALLBACK _callback = NULL)
			: AscenderOption(_text, _marginTop, _marginBottom, _callback), target(_target), value(_value) {};
		
		void		action(int keyScancode)
		{
			(*target) = value;
			AscenderOption::action(keyScancode);
		};
};

/******************************************************/

#endif
