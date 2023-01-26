#ifndef _MAINMENU_H
#define _MAINMENU_H

/**************************************/

#include "Classes.h"

/**************************************/

class MainMenu
{
	protected:
		
		Menu *mMain, *mOptions, *mDetails, *mGameplay, *mSound, *mAccess;

		Menu *mBattle, *mFrag;
		int	rawPlayerCount, rawRoundCount, rawFragCount, rawCompPlayerCount;
		int	rawCompSkill;
	
		Menu *mTarget;
		int   rawPlayTime, rawTimeBonus;
		
		TimeKeeper *TK;
	
		void		assignController(Player *player);
	
		void		playTargetGame();
		void		playBattleGame();
		void		playFragGame();
	
	public:
		
		MainMenu();
		~MainMenu();

		void go();
};

/**************************************/

#endif
