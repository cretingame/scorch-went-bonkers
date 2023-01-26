#include <cstdio>
#include <GL/gl.h>
#include <alleggl.h>
#include <allegro.h>

#include "../control/TankControl.h"
#include "../decor/Cloud.h"
#include "../decor/TextFloat.h"
#include "../engine/TimeKeeper.h"
#include "../entity/GameWorld.h"
#include "../player/Player.h"
#include "../objects/Background.h"
#include "../objects/Terrain.h"
#include "../objects/Tank.h"
#include "../objects/Scoreboard.h"
#include "../objects/Target.h"
#include "../misc/Random.h"
#include "../misc/KeyReader.h"
#include "../misc/Settings.h"
#include "../misc/ScreenFader.h"
#include "../misc/Theme.h"
#include "../misc/Quitting.h"
#include "../sound/SoundSystem.h"

#include "../menu/StaticText.h"
#include "../menu/Menu.h"

#include "FragGame.h"

/***********************************************/

FragGame::FragGame(Player **_players, int _playerCount, int _fragLimit)
	: Game(_players, _playerCount), fragLimit(_fragLimit)
{
	theme = NULL;
	background = NULL;
	terrain = NULL;
	world = NULL;
	scoreboard = NULL;
	
	tanks = new Tank*[playerCount];
}

FragGame::~FragGame()
{

	delete [] tanks;
	
	if (background)
		delete background;
	
	if (theme)
		delete theme;
	
	if (scoreboard)
		delete scoreboard;
}

/***********************************************/

void FragGame::shufflePlayers()
{
	Player *temp;
	int nr1, nr2;
	
	for (int i = 0; i < randomInt(35, 44); i++)
	{
		nr1 = randomInt(0, playerCount - 1);
		nr2 = nr1;
		while (nr2 == nr1)
			nr2 = randomInt(0, playerCount - 1);
		
		temp = players[nr2];
		players[nr2] = players[nr1];
		players[nr1] = temp;
	}
}

/***********************************************/

bool FragGame::checkForRoundEnd()
{
	// does anyone have enough frags?	
	for (int i = 0; i < playerCount; i++)
		if (players[i]->getScore() >= fragLimit) {
			return true;
		}

	return false;
}

/***********************************************/

void FragGame::respawnPlayers()
{
	// check for dying players
	for (int i = 0; i < playerCount; i++)
		if (!tanks[i]->isAlive()) {
			// this tank is dead

			// this will change somebody's score
			Player *tanksOwner = players[i];
			Player *causeOfDeath = tanks[i]->getLastDamagedBy();
			if (causeOfDeath == tanksOwner) 
			{
				// suicide
				tanksOwner->addScore(-1);
			}
			else if (causeOfDeath != NULL)
			{
				// deliberate murder, score a frag
				causeOfDeath->addScore(1);
				Tank *hisTank = causeOfDeath->getCurrentTank();
				world->add(new TextFloat(hisTank->getX(), hisTank->getY() - 8.0, "+FRAG", GLColor(1.0, 1.0, 0.5), 0.25), true);
			}

			// find a place to respawn this tank						
			double xCoord;
			bool positionOK = true;
			do {
				xCoord = randomFloat(40.0, 760.0);
				positionOK = true;
				for (int j = 0; j < playerCount; j++)
					if (abs(tanks[j]->getX() - xCoord) < 68.0)
						positionOK = false;	
			} while (!positionOK);

			// place the tank
			terrain->flattenAt(xCoord);
			tanks[i]->respawn(world, xCoord, 594.0 - terrain->getHeightAt(xCoord));			
		}	

}

/***********************************************/

void FragGame::drawEverything()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	world->processRender();
	
	allegro_gl_flip();
}

/********************************************/

void FragGame::finalStandings()
{
	/** BUBBLE SORT THE PLAYERS :) **/
	
	Player *t;
	for (int i = 0; i < playerCount; i++)
		for (int j = 0; j < playerCount - 1; j++)
			if (players[j]->getScore() < players[j+1]->getScore())
			{
				t = players[j];
				players[j] = players[j+1];
				players[j+1] = t;
			}
	
	/** INFO **/
	
	static char buf[200];
	Menu *info = new Menu(400, 300);
	
	info->addOption(new StaticText("We have a winner!", GLColor(1.0, 1.0, 1.0), 0.0, 40.0, 0.8),
		Effect(0.0, 40.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	sprintf(buf, "After a fierce battle, the winner is:");
	info->addOption(new StaticText(buf, GLColor(1.0, 1.0, 1.0)),
		Effect(0.0, 40.0, E_BOTTOM), Effect(8.0, 30.0, E_ENLARGE));
	
	info->addOption(new StaticText(players[0]->getName().c_str(), players[0]->getColor(), 50.0, 50.0, 1.0),
		Effect(40.0, 40.0, E_ENLARGE), Effect(16.0, 30.0, E_ENLARGE));
	
	info->addOption(new StaticText("The final standings were:", GLColor(1.0, 1.0, 1.0), 15.0, 30.0),
		Effect(70.0, 30.0, E_BOTTOM), Effect(24.0, 30.0, E_ENLARGE));
	
	for (int i = 0; i < playerCount; i++)
	{
		sprintf(buf, "%s - %d", players[i]->getName().c_str(), players[i]->getScore());
		info->addOption(new StaticText(buf, players[i]->getColor(), 15.0, 15.0, 0.5),
			Effect(90.0 + 8.0 * i, 30.0, E_ENLARGE), Effect(32.0 + 8.0 * i, 30.0, E_ENLARGE));
	}
			
	info->addOption(new StaticText("Everyone, please press your buttons to continue.", GLColor(1.0, 1.0, 1.0), 45.0, 0.0, 0.3),
			Effect(20.0, 40.0, E_RIGHT), Effect(24.0, 30.0, E_ENLARGE));
	
	displayInfo(info);
	
	delete info;
}

/***********************************************/

RoundEndReason FragGame::playOneRound()
{
	double gameSpeedModifier = 1.0 + Settings::setting[GPL_GAME_SPEED] * 0.5;
	/** ROUND INITIALIZATION **/
	
	if (!theme)
	{
		theme = Theme::load("greens");
		if (!theme)
		{
			allegro_message("Unable to load theme.");
			return RND_PLAYERS_QUIT;
		}
	}
	if (!background)
		background = new Background(theme);
	if (!scoreboard)
		scoreboard = new Scoreboard(players, playerCount);
	else
		scoreboard->step(0.0);
	
	world = new GameWorld();
	terrain = new Terrain();
	
	shufflePlayers();
	for (int i = 0; i < playerCount; i++)
	{
		double xCoord = 100.0 + 600.0 * i / (playerCount - 1);
		xCoord = randomFloat(xCoord - 10.0, xCoord + 10.0);
		
		terrain->flattenAt(xCoord);
		
		tanks[i] = players[i]->getNewTank();
		tanks[i]->respawn(world, xCoord, terrain->getYAt(xCoord));
	}
	
	world->setPauseImmediate(PS_PAUSED);
	world->add(background, false);
	world->add(scoreboard, false);
	world->add(terrain, true);
	
	if (Settings::setting[GFX_CLOUDS])
	{
		for (int i = 0; i < theme->getMaxClouds(); i++)
		{
			Cloud *c = new Cloud(theme, randomInt(0, theme->getCloudsTypeCount() - 1), false);
			c->setX(i * 800.0 / theme->getMaxClouds() + randomFloat(-20.0, 20.0) - 50.0);
			world->add(c, true);
		}
	}
	
	ScreenFader::fadeToColor(theme->getBackColor(), 15.0);

	/** DURING THE ROUND **/
	
	RoundEndReason roundState = RND_DIDNT_END;
	double delta;
		
	world->togglePause();
	TK->reset();
		
	while(roundState == RND_DIDNT_END)
	{		
		// close button handling
		if (Quitting::applicationQuitting())
			return RND_APP_QUIT;

		delta = TK->getDT() * 100; // we want delta in 0.01s
		do 
		{
			// logic
			world->processCollisions();
			if (world->getPauseState() == PS_PLAYING)
				world->processStep(gameSpeedModifier * delta);
			else
				world->processStep(2.0 * delta);

			
			if (checkForRoundEnd())
				roundState = RND_NATURAL;
			if (roundState == RND_DIDNT_END)
				respawnPlayers();
			
			soundSys->setMusicVolume((int)(world->getPauseTransition() * world->getPauseTransition() * 100));

			KeyReader::update();
			
			for(KEY_ITERATOR k = KeyReader::scancodes.begin(); k != KeyReader::scancodes.end(); k++)
			{
				int scancode = (*k);
				switch(scancode)
				{
					case KEY_P:
						world->togglePause();
						break;
					case KEY_ESC:
						roundState = RND_PLAYERS_QUIT;
						break;
				}
			}
		}while(TK->getFramesToSkip() > 0);

		// drawing
		drawEverything();
	}
	
	/** AFTER THE ROUND **/

	world->togglePause();
	while (world->getPauseState() != PS_PAUSED)
	{
		delta = TK->getDT() * 100; // we want delta in 0.01s
		do 
		{
			soundSys->setMusicVolume((int)(world->getPauseTransition() * world->getPauseTransition() * 100));
			world->processStep(2.0 * delta);
		} while(TK->getFramesToSkip() > 0);
		
		drawEverything();
	}
	
	delete world;
	
	return roundState;
}

/********************************************/

void FragGame::play()
{
	GameEndReason end = GAME_DIDNT_END;
	
	while(end == GAME_DIDNT_END)
	{
		RoundEndReason reason = playOneRound();
		
		if (reason == RND_APP_QUIT)
			return;
		if (reason == RND_PLAYERS_QUIT)
			end = GAME_PLAYERS_QUIT;		
		if (reason == RND_NATURAL)
			end = GAME_NATURAL;
	}
	
	if (end == GAME_NATURAL)
		finalStandings();
}
