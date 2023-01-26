#ifndef _KEYSELECTMENU_H
#define _KEYSELECTMENU_H

/**************************************/

#include "../Classes.h"
#include "../menu/Menu.h"

/**************************************/

class KeySelectMenu
	: public Menu
{
	protected:
		TankControl *control;
	
	public:
		KeySelectMenu(double _x, double _y);
	
		TankControl *getController() {return control;};
	
		void	checkKeys();
};

/**************************************/

#endif
