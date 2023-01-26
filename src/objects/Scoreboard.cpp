#include "../misc/TextHandler.h"
#include "../player/Player.h"
#include "Scoreboard.h"

/****************************************************/

Scoreboard::Scoreboard(Player **_players, int _playerCount)
	: Entity(790.0, 7.0), playerCount(_playerCount) 
{
	players = new Player*[playerCount]; oldScore = new int[playerCount];
	
	for (int i = 0; i < playerCount; i++)
	{
		players[i] = _players[i];
		oldScore[i] = players[i]->getScore();
	}
	
	sort();
};

Scoreboard::~Scoreboard()
{
	delete[] players; delete[] oldScore;
}

/****************************************************/

void Scoreboard::sort()
{
	for (int i = 0; i < playerCount - 1; i++)
		for (int j = i; j < playerCount - 1; j++)
			if (players[j]->getScore() < players[j + 1]->getScore())
			{
				int tScore;
				Player *tPlayer;
				
				tPlayer = players[j];
				tScore = oldScore[j];
				players[j] = players[j + 1];
				oldScore[j] = oldScore[j + 1];
				players[j + 1] = tPlayer;
				oldScore[j + 1] = tScore;
			}
}

void Scoreboard::step(double /*dt*/)
{
	for (int i = 0; i < playerCount; i++)
		if (players[i]->getScore() != oldScore[i])
		{
			sort();
			break;
		}
}

void Scoreboard::render()
{
	glDisable(GL_ALPHA_TEST);
	
	TextRenderer *tr = TextHandler::getInstance()->getRenderer("wet");
	
	tr->setSize(0.4, 0.4);

	for (int i = 0; i < playerCount; i++)
	{
		tr->printf((int)x + 1, (int)y + i * 18 + 1, 1.0, 0.0, GLColor(0.0, 0.0, 0.0, 0.4), "%s: %d", players[i]->getName().c_str(), players[i]->getScore());
	
		GLColor printColor = players[i]->getColor();
		printColor.a = 0.8;
		
		tr->printf((int)x, (int)y + i * 18, 1.0, 0.0, printColor, "%s: %d", players[i]->getName().c_str(), players[i]->getScore());
	}
}

