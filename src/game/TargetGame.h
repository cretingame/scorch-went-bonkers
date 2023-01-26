#ifndef _TARGETGAME_H
#define _TARGETGAME_H

/****************************************/

#include "../Classes.h"
#include "../game/Game.h"

/****************************************/

const int TARGET_COUNT = 3;

/****************************************/

class TargetGame
	: public Game
{
	protected:
		
		bool		targetHit[TARGET_COUNT];
	
		Theme		*theme;
		Background	*background;
		GameWorld	*world;
		Tank		*tank;
		Terrain	*terrain;
		Target	*targets[TARGET_COUNT];
		TargetBoard *targetBoard;
		TimeCounter *timeCount;
	
		double	curTime, timeLimit;
		double	timeBonus;
	
		double	randomTargetCoord();
		void		maintainTargets();
		void		drawEverything();
	
		RoundEndReason		playOneRound();
		void				afterGameScreen();

	public:
		
		TargetGame(Player **_players, double _timeLimit, double _timeBonus);
		~TargetGame();
	
		void		play();
};

/****************************************/

#endif
