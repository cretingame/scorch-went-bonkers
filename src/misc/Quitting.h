#ifndef _QUITTING_H
#define _QUITTING_H

/************************************************/

class Quitting {
	private:
		static bool 	quitFlag;

	public:
		static void		registerHandler();

		static void		triggerQuit();
		static bool		applicationQuitting();
};

/************************************************/

#endif