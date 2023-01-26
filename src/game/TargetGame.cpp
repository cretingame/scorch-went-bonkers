#include <cmath>
#include <cstdio>
#include <GL/gl.h>
#include <alleggl.h>

#include "../decor/Cloud.h"
#include "../decor/TextFloat.h"
#include "../entity/GameWorld.h"
#include "../objects/Target.h"
#include "../objects/Terrain.h"
#include "../objects/Background.h"
#include "../objects/TargetBoard.h"
#include "../objects/Tank.h"
#include "../objects/TimeCounter.h"
#include "../misc/Random.h"
#include "../misc/KeyReader.h"
#include "../misc/Settings.h"
#include "../misc/Theme.h"
#include "../misc/ScreenFader.h"
#include "../misc/Quitting.h"
#include "../menu/Menu.h"
#include "../menu/StaticText.h"
#include "../player/Player.h"
#include "../engine/TimeKeeper.h"
#include "../sound/SoundSystem.h"

#include "TargetGame.h"

/************************************************/

TargetGame::TargetGame(Player **_players, double _timeLimit, double _timeBonus)
	: Game(_players, 1), timeLimit(_timeLimit * 100.0), timeBonus(_timeBonus * 100.0)
{
	theme = NULL;
	background = NULL;
	terrain = NULL;
	world = NULL;
	targetBoard = NULL;
}

/************************************************/

TargetGame::~TargetGame()
{
	if (theme)
		delete theme;
	
	if (background)
		delete background;
	
	if (targetBoard)
		delete targetBoard;
}

/************************************************/

void TargetGame::drawEverything()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	world->processRender();
	
	allegro_gl_flip();
}

/************************************************/

double TargetGame::randomTargetCoord()
{
	int dir;
	double coord;
	bool targetOK;
	
	do
	{
		dir = randomInt(0, 1);
		coord = (dir == 0) ? randomFloat(50.0, 250.0) : randomFloat(550.0, 750.0);
		
		targetOK = true;
		for (int i = 0; i < TARGET_COUNT; i++)
			if (targets[i] && std::abs(targets[i]->getX() - coord) < 40.0)
				targetOK = false;
	} while (!targetOK);
	
	return coord;
}

/************************************************/

void TargetGame::maintainTargets()
{
	char buf[10];
	
	for (int i = 0; i < TARGET_COUNT; i++)
	{
		if (targets[i]->isHit() && (!targetHit[i]))
		{
			targetHit[i] = true;
			
			players[0]->addScore(1);
			timeLimit += timeBonus;
			timeCount->setLimit(timeCount->getLimit() + timeBonus);
			targetBoard->addHit();
		}
		
		if (!targets[i]->isAlive())
		{
			if (timeLimit < 360000.0)
			{
				sprintf(buf, "+%d SEC", (int)(timeBonus / 100.0));
				world->add(new TextFloat(targets[i]->getX(), targets[i]->getY(), buf, GLColor(0.0, 1.0, 0.0)), true);
			}
			
			double x = randomTargetCoord();
			delete targets[i];
			targets[i] = new Target(x, world, 40.0);
			targetHit[i] = false;
			world->add(targets[i], false);
		}
	}
}

/************************************************/

RoundEndReason TargetGame::playOneRound()
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
	if (!targetBoard)
		targetBoard = new TargetBoard();
	
	world = new GameWorld();
	terrain = new Terrain();
	
	terrain->flattenAt(400.0);
	tank = players[0]->getNewTank();
	tank->setX(400.0);
	tank->setY(594.0 - terrain->getHeightAt(400.0));
	tank->setInvincible(true);
	
	timeCount = new TimeCounter(timeLimit, gameSpeedModifier);
	
	world->add(background, false);
	world->add(targetBoard, false);
	world->add(terrain, true);
	world->add(tank, false);
	world->add(timeCount, true);
	
	if (Settings::setting[GFX_CLOUDS])
		for (int i = 0; i < theme->getMaxClouds(); i++)
		{
			Cloud *c = new Cloud(theme, randomInt(0, theme->getCloudsTypeCount() - 1), false);
			c->setX(i * 800.0 / theme->getMaxClouds() + randomFloat(-20.0, 20.0) - 50.0);
			world->add(c, true);
		}

	
	for (int i = 0; i < TARGET_COUNT; i++)
		targets[i] = NULL;
	
	for (int i = 0; i < TARGET_COUNT; i++)
	{
		targets[i] = new Target(randomTargetCoord(), world, 40.0);
		targetHit[i] = false;
		world->add(targets[i], false);
	}
	
	world->setPauseImmediate(PS_PAUSED);

	curTime = 0.0;
	
	/** DURING THE ROUND **/
	
	RoundEndReason roundState = RND_DIDNT_END;
	int prevTankState = TS_IDLE;
	
	ScreenFader::fadeToColor(theme->getBackColor(), 15.0);
	
	double delta;
	world->togglePause();
	TK->reset();
	
	while(roundState == RND_DIDNT_END)
	{		
		// close button handling
		if (Quitting::applicationQuitting()) {
			return RND_APP_QUIT;
		}

		delta = TK->getDT() * 100; // we want delta in 0.01s
		do 
		{
			// logic
			world->processCollisions();
			if (world->getPauseState() == PS_PLAYING)
			{
				world->processStep(gameSpeedModifier * delta);
				curTime += delta;
			}
			else
				world->processStep(2.0 * delta);
			
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

			soundSys->setMusicVolume((int)(world->getPauseTransition() * world->getPauseTransition() * 100));
			
			if (curTime >= timeLimit)
			{
				soundSys->playSound(SND_TIME_BUZZ, 128, 0.4, 0.8);
				roundState = RND_NATURAL;
			}
			
			if (tank->getState() != prevTankState)
			{
				if (prevTankState == TS_SHOOTING)
					targetBoard->addFired();
				
				prevTankState = tank->getState();
			}
			
			if (roundState == RND_DIDNT_END)
				maintainTargets();
			
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
			world->processStep(2.0 * delta);
			soundSys->setMusicVolume((int)(world->getPauseTransition() * world->getPauseTransition() * 100));
			
		} while(TK->getFramesToSkip() > 0);
		
		drawEverything();
	}
	
	delete world;
	for (int i = 0; i < TARGET_COUNT; i++)
		if (targets[i])
		{
			delete targets[i];
			targets[i] = NULL;
		}
	
	return roundState;
}

/************************************************/

void TargetGame::afterGameScreen()
{
	char buf[200];
	Menu *info = new Menu(400, 300);
	
	info->addOption(new StaticText("Time's up!", GLColor(1.0, 1.0, 1.0), 0.0, 40.0, 0.8),
			Effect(0.0, 40.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	sprintf(buf, "After %d seconds, your results are:", (int)(timeLimit / 100.0));
	info->addOption(new StaticText(buf, GLColor(1.0, 1.0, 1.0), 15.0, 45.0));
	
	sprintf(buf, "%d destroyed targets", players[0]->getScore());
	info->addOption(new StaticText(buf, GLColor(1.0, 1.0, 0.0), 20.0, 20.0, 0.6));
	
	sprintf(buf, "%d shots fired", targetBoard->getFired());
	info->addOption(new StaticText(buf, GLColor(1.0, 0.9, 0.0), 20.0, 20.0, 0.6));
	
	sprintf(buf, "%.1lf%% accuracy", targetBoard->getAccuracy());
	info->addOption(new StaticText(buf, GLColor(1.0, 0.8, 0.0), 20.0, 20.0, 0.6));
	
	info->addOption(new StaticText("Press something to continue.", GLColor(1.0, 1.0, 1.0), 45.0, 0.0, 0.3),
			Effect(40.0, 40.0, E_BOTTOM), Effect(40.0, 30.0, E_ENLARGE));
	
	displayInfo(info, true);
	
	delete info;
}

/************************************************/

void TargetGame::play()
{
	RoundEndReason reason = playOneRound();
	
	if (reason == RND_NATURAL)
		afterGameScreen();
}
