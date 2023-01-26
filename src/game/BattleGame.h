#ifndef _BATTLEGAME_H
#define _BATTLEGAME_H

/***************************************************/

#include "../Classes.h"
#include "../game/Game.h"

/***************************************************/

class BattleGame
	: public Game
{
	protected:
	
		int		pointLimit;
	
		Theme		*theme;
		Background	*background;
		GameWorld	*world;
		Tank		**tanks;
		Terrain	*terrain;
		Scoreboard	*scoreboard;
	
	
		bool				checkForRoundEnd();
		void				drawEverything();
		void				shufflePlayers();
	
		void				afterRoundScreen(int roundNr, RoundEndReason reason, Player *winner);
		void				finalStandings(int roundCount);
	
		Player*			scoreRound();
		RoundEndReason		playOneRound();

	public:
		BattleGame(Player **_players, int _playerCount, int _pointLimit);
		~BattleGame();

		void		play();
};

/***************************************************/

#endif
