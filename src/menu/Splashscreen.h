#ifndef _SPLASHSCREEN_H
#define _SPLASHSCREEN_H

/*********************************************/

#include "../Classes.h"

/*********************************************/

class SplashScreen
{
	protected:
		
		Menu		*screen[3];

		bool		displayScreen(Menu *menu, double delay, double lasts);
	
	public:
		SplashScreen();
		~SplashScreen();
	
		void		play();
};

/*********************************************/

#endif
