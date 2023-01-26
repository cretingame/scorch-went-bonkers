#ifndef _MENUOPTION_H
#define _MENUOPTION_H

/****************************************/

#include "../Classes.h"
#include "../engine/GLColor.h"

#include <string>
using namespace std;

/****************************************/

typedef void(*ACTION_CALLBACK)(MenuOption*);

/****************************************/

enum MenuDirection {MDIR_UP, MDIR_DOWN, MDIR_NONE};

/****************************************/

class MenuOption
{
	protected:
		
		string		text;
		double		marginTop, marginBottom;
	
		GLColor		textColor, shadowColor;
		double		textSize;
	
		Menu			*parentMenu;
		bool			selected;
	
		TextRenderer	*textRend;
	
		ACTION_CALLBACK	callback;
	
	public:
		MenuOption(const char *_text, double _marginTop = 15.0, double _marginBottom = 15.0, ACTION_CALLBACK _callback = NULL);
		virtual ~MenuOption(){};
	
		virtual bool	isSelectible() {return true;};
		
		virtual void	setParentMenu(Menu *_parent) {parentMenu = _parent;};
		virtual Menu*	getParentMenu() {return parentMenu;};
		
		double		getMarginTop() {return marginTop;};
		double		getMarginBottom() {return marginBottom;};
		
		virtual void	select(int fromDirection);
		virtual void	deselect(int toDirection);
		
		virtual bool	respondsToLeftRight() {return false;};
		virtual void	action(int keyScancode);
		
		virtual void	renderAt(double x, double y);
};

/****************************************/

#endif
