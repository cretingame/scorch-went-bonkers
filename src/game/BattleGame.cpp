#include <cstdio>
#include <GL/gl.h>
#include <alleggl.h>
#include <allegro.h>

#include "../control/TankControl.h"
#include "../decor/Cloud.h"
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

#include "BattleGame.h"

/***********************************************/

BattleGame::BattleGame(Player **_players, int _playerCount, int _pointLimit)
	: Game(_players, _playerCount), pointLimit(_pointLimit)
{
	theme = NULL;
	background = NULL;
	terrain = NULL;
	world = NULL;
	scoreboard = NULL;
	
	tanks = new Tank*[playerCount];
}

BattleGame::~BattleGame()
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

void BattleGame::shufflePlayers()
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

bool BattleGame::checkForRoundEnd()
{
	int tanksAlive = 0;
	
	for (int i = 0; i < playerCount; i++)
		if (tanks[i]->isAlive())
			tanksAlive++;
	
	if (tanksAlive > 1)
		return false;
	
	if (   (world->countEntitiesOfType(ETYPE_BULLET) == 0)
	    && (world->countEntitiesOfType(ETYPE_EXPLOSION) == 0) )
		return true;
	else
		return false;
}

/***********************************************/

void BattleGame::drawEverything()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	world->processRender();
	
	allegro_gl_flip();
}

/***********************************************/

Player* BattleGame::scoreRound()
{
	for (int i = 0; i < playerCount; i++)
		if (tanks[i]->isAlive())
		{
			players[i]->addScore(1);
			return players[i];
		}
	
	return NULL;
}

/***********************************************/

void BattleGame::afterRoundScreen(int roundNr, RoundEndReason reason, Player *winner)
{
	/** Prepare the info **/
	
	static char buf[200];
	const char *winnerText;
	GLColor winnerColor;
	
	Menu *info = new Menu(400, 300);
	
	sprintf(buf, "Round %d finished", roundNr);
	info->addOption(new StaticText(buf, GLColor(0.9, 0.9, 0.9), 0.0, 40.0, 0.8),
			Effect(0.0, 40.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	info->addOption(new StaticText("And the winner is:", GLColor(1.0, 1.0, 1.0)),
			Effect(20.0, 40.0, E_LEFT), Effect(8.0, 30.0, E_ENLARGE));
	
	if (winner)
	{
		winnerText = winner->getName().c_str();
		winnerColor = winner->getColor();
	}
	else
	{
		winnerText = "Nobody";
		winnerColor = GLColor(0.9, 0.9, 0.9);
	}
	info->addOption(new StaticText(winnerText, winnerColor, 25.0, 25.0, 0.6),
			Effect(60.0, 30.0, E_ENLARGE), Effect(16.0, 30.0, E_ENLARGE));
	
	info->addOption(new StaticText("Everyone, please press your buttons to continue.", GLColor(1.0, 1.0, 1.0), 45.0, 0.0, 0.3),
			Effect(20.0, 40.0, E_RIGHT), Effect(24.0, 30.0, E_ENLARGE));
	
	/** Display the prepared info **/
	
	displayInfo(info);
	
	/** Get rid of it **/
	delete info;
}

/********************************************/

void BattleGame::finalStandings(int roundCount)
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
	
	info->addOption(new StaticText("Battle ended", GLColor(1.0, 1.0, 1.0), 0.0, 40.0, 0.8),
		Effect(0.0, 40.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	sprintf(buf, "After %d round(s) of fierce battle, the winner is:", roundCount);
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

RoundEndReason BattleGame::playOneRound()
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
		tanks[i]->setX(xCoord);
		tanks[i]->setY(594.0 - terrain->getHeightAt(xCoord));
	}
	
	world->setPauseImmediate(PS_PAUSED);
	world->add(background, false);
	world->add(scoreboard, false);
	world->add(terrain, true);
	for (int i = 0; i < playerCount; i++)
		world->add(tanks[i], false);
	
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

void BattleGame::play()
{
	GameEndReason end = GAME_DIDNT_END;
	
	int roundNr = 1;
	Player *winner;
	
	while(end == GAME_DIDNT_END)
	{
		RoundEndReason reason = playOneRound();
		
		if (reason == RND_APP_QUIT)
			return;

		if (reason == RND_PLAYERS_QUIT)
			end = GAME_PLAYERS_QUIT;
		
		if (reason == RND_NATURAL)
			winner = scoreRound();
		else
			winner = NULL;
		
		for(int i = 0; i < playerCount; i++)
			if (players[i]->getScore() >= pointLimit)
				end = GAME_NATURAL;
		
		if (end == GAME_DIDNT_END)
			afterRoundScreen(roundNr, reason, winner);
		
		roundNr++;
	}
	
	if (end == GAME_NATURAL)
		finalStandings(roundNr - 1);
}
