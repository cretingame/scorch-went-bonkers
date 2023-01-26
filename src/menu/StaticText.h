#ifndef _STATICTEXT_H
#define _STATICTEXT_H

/*********************************************/

#include "../menu/MenuOption.h"

/*********************************************/

class StaticText
	: public MenuOption
{
	protected:
	public:
		StaticText(const char *_text, GLColor _textColor, double _marginTop = 15.0, double _marginBottom = 15.0, 
				double _textSize = 0.4)
			: MenuOption(_text, _marginTop, _marginBottom)
		{
			textColor = _textColor;
			textSize = _textSize;
		};
		virtual ~StaticText(){};
			
		bool			isSelectible() {return false;};
		
		void select(int fromDir);
		void deselect(int toDir);
};

/*********************************************/

#endif
