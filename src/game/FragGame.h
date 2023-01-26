#ifndef _FRAGGAME_H
#define _FRAGGAME_H

/***************************************************/

#include "../Classes.h"
#include "../game/Game.h"

/***************************************************/

class FragGame
	: public Game
{
	protected:
	
		int		fragLimit;
	
		Theme		*theme;
		Background	*background;
		GameWorld	*world;
		Tank		**tanks;
		Terrain	*terrain;
		Scoreboard	*scoreboard;
	
	
		void				respawnPlayers();
		bool				checkForRoundEnd();

		void				drawEverything();
		void				shufflePlayers();
	
		void				finalStandings();

		RoundEndReason		playOneRound();

	public:
		FragGame(Player **_players, int _playerCount, int _pointLimit);
		~FragGame();

		void		play();
};

/***************************************************/

#endif
