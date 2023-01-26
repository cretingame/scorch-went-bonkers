#ifndef _GAME_H
#define _GAME_H

/****************************************/

#include "../Classes.h"

/****************************************/

enum RoundEndReason {RND_DIDNT_END, RND_PLAYERS_QUIT, RND_DRAW, RND_NATURAL, RND_APP_QUIT};
enum GameEndReason  {GAME_DIDNT_END, GAME_PLAYERS_QUIT, GAME_NATURAL, GAME_APP_QUIT};

/****************************************/

class Game
{
	protected:
		
		TimeKeeper		*TK;
	
		Player		**players;
		int			playerCount;
	
		void		displayInfo(Menu *info, bool anyKey = false);
	
	public:
		Game(Player **_players, int _playerCount);
		~Game();
};

/****************************************/

#endif
