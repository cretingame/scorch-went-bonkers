#ifndef _SCOREBOARD_H
#define _SCOREBOARD_H

/********************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/********************************************/

class Scoreboard
	: public Entity
{
	protected:
		Player **players;
		int *oldScore;
		int playerCount;
	
	public:

		Scoreboard(Player **_players, int _playerCount);
		~Scoreboard();
	
		int		stepPriority() {return 1;};
		int		renderPriority() {return 1;};
		double	scrollingSpeed() {return -0.4;};
		void		render();
		void		step(double dt);
		
		void		sort();
};

/********************************************/

#endif
