#include <alleggl.h>
#include <GL/gl.h>

#include "../engine/TimeKeeper.h"
#include "../misc/KeyReader.h"
#include "../misc/ScreenFader.h"
#include "../menu/Menu.h"
#include "../sound/SoundSystem.h"
#include "../player/Player.h"
#include "../control/TankControl.h"

#include "Game.h"

/**************************************/

Game::Game(Player **_players, int _playerCount)
	: players(_players), playerCount(_playerCount)
{
	TK = new TimeKeeper(0.01, 0.025, 2, true);
}

Game::~Game()
{
	for (int i = 0; i < playerCount; i++)
		delete players[i];
	
	delete TK;
}

/**************************************/

void Game::displayInfo(Menu *info, bool anyKey)
{
	TK->reset();
	
	double delta;
	int done = 0;
	
	bool *pressed = new bool[playerCount];
	for (int i = 0; i < playerCount; i++)
		pressed[i] = false;
	
	ScreenFader::fadeToColor(GLColor(55, 157, 255), 15.0);
	
	KeyReader::clearBuffer();
	info->animFadeTo(AS_FINISHED);
	
	while ((done != playerCount) || (info->getAnimState() != AS_INVISIBLE))
	{
		delta = TK->getDT() * 100;
		do 
		{
			info->step(delta);
			
			for (int i = 0; i < playerCount; i++)
				if ((players[i]->getController()->checkPress()) && (!pressed[i]))
				{
					pressed[i] = true;
					done++;
					soundSys->playSound(SND_MENU_SELECT, 128, 0.5, 1.0);
					
					if (done == playerCount)
						info->animFadeTo(AS_INVISIBLE);
				}
			
			KeyReader::update();
			if (anyKey && done != playerCount)
			{
				bool click = false;
				
				if (!KeyReader::scancodes.empty())
					click = true;
				
				if (mouse_b != 0)
					click = true;
				
				for (int joyNr = 0; joyNr < num_joysticks; joyNr++)
					for (int buttNr = 0; buttNr < joy[joyNr].num_buttons; buttNr++)
						if (joy[joyNr].button[buttNr].b)
							click = true;
				if (click)
				{
					info->animFadeTo(AS_INVISIBLE);
					done = playerCount;
				}
			}
			
		} while(TK->getFramesToSkip() > 0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		info->render();
		allegro_gl_flip();
	}
	
	delete [] pressed;
}

/***************************************************/
