#include <iostream>
#include <alleggl.h>
#include <GL/gl.h>

#include "../engine/TimeKeeper.h"
#include "../engine/GLBitmap.h"
#include "../menu/Menu.h"
#include "../menu/StaticText.h"
#include "../menu/StaticImage.h"
#include "../misc/KeyReader.h"

#include "SplashScreen.h"

/****************************************************/

SplashScreen::SplashScreen()
{
	GLBitmap *retroRemakesBMP = GLBitmap::load("gfx/splash/retro_remakes.tga", GLBitmap::TF_NO_ALPHA);
	
	/*****************************/
	
	screen[0] = new Menu(400, 300);
	
	screen[0]->addOption(new StaticText("Jakub Wasilewski", GLColor(1.0, 1.0, 0.0), 40.0, 40.0, 0.8),
		Effect(0.0, 50.0, E_WILD), Effect(0.0, 50.0, E_WILD));
	screen[0]->addOption(new StaticText("presents", GLColor(1.0, 0.8, 0.0), 20.0, 20.0, 0.5),
		Effect(20.0, 30.0, E_BOTTOM), Effect(20.0, 30.0, E_ENLARGE));
	
	/*****************************/
	
	screen[1] = new Menu(400, 300);
	
	screen[1]->addOption(new StaticText("A not altogether serious game", GLColor(1.0, 1.0, 0.6), 20.0, 20.0, 0.5),
		Effect(0.0, 40.0, E_LEFT), Effect(0.0, 40.0, E_ENLARGE));
	screen[1]->addOption(new StaticText("about tanks", GLColor(1.0, 1.0, 0.3), 20.0, 20.0, 0.5),
		Effect(80.0, 40.0, E_RIGHT), Effect(10.0, 40.0, E_ENLARGE));
	screen[1]->addOption(new StaticText("and bullets, and... umm, stuff", GLColor(1.0, 1.0, 0.0), 20.0, 20.0, 0.5),
		Effect(160.0, 40.0, E_LEFT), Effect(20.0, 40.0, E_ENLARGE));
	
	screen[1]->addOption(new StaticText("Created for the", GLColor(1.0, 0.9, 0.0), 60.0, 20.0, 0.5),
		Effect(240.0, 40.0, E_BOTTOM), Effect(30.0, 40.0, E_ENLARGE));
	screen[1]->addOption(new StaticImage(retroRemakesBMP, 30.0, 30.0),
		Effect(270.0, 40.0, E_BOTTOM), Effect(40.0, 40.0, E_ENLARGE));
	screen[1]->addOption(new StaticText("and one-switch.org competition in 2005", GLColor(1.0, 0.8, 0.0), 20.0, 20.0, 0.5),
		Effect(300.0, 40.0, E_BOTTOM), Effect(50.0, 40.0, E_ENLARGE));
	
	/*****************************/
	
	screen[2] = new Menu(400,300);
	
	screen[2]->addOption(new StaticText("Featuring an old Amiga song", GLColor(1.0, 1.0, 0.9), 20.0, 20.0, 0.5),
		Effect(0.0, 40.0, E_TOP), Effect(20.0, 40.0, E_ENLARGE));
	screen[2]->addOption(new StaticText("''Crack the eggshell''", GLColor(1.0, 1.0, 0.1), 25.0, 25.0, 0.6),
		Effect(40.0, 40.0, E_ENLARGE), Effect(0.0, 40.0, E_ENLARGE));
	screen[2]->addOption(new StaticText("by Jester", GLColor(1.0, 1.0, 0.9), 20.0, 20.0, 0.5),
		Effect(0.0, 40.0, E_BOTTOM), Effect(20.0, 40.0, E_ENLARGE));
}

SplashScreen::~SplashScreen()
{
	for(int i = 0; i < 3; i++)
		delete screen[i];
}

/****************************************************/

bool SplashScreen::displayScreen(Menu *screen, double delay, double lasts)
{
	TimeKeeper *TK = new TimeKeeper(0.01, 0.025, 2, true);
	TK->reset();
	
	double delta, time = 0.0;
	while(time < delay)
	{
		time += TK->getDT() * 100;
		glClear(GL_COLOR_BUFFER_BIT);
		allegro_gl_flip();
		
		KeyReader::update();
		if (!KeyReader::scancodes.empty())
			return true;
	}
	
	time = 0.0;
	screen->animFadeTo(AS_FINISHED);
	while ((time < lasts) || (screen->getAnimState() != AS_INVISIBLE))
	{
		delta = TK->getDT() * 100;
		time += delta;
		
		if (time >= lasts && screen->getAnimState() == AS_FINISHED)
			screen->animFadeTo(AS_INVISIBLE);
		
		do 
		{
			screen->step(delta);
			
			KeyReader::update();
			if (!KeyReader::scancodes.empty())
				return true;
			
		} while(TK->getFramesToSkip() > 0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		screen->render();
		allegro_gl_flip();
	}
	
	delete TK;
	
	return false;
}

/****************************************************/

void SplashScreen::play()
{
	if (displayScreen(screen[0], 100.0, 360.0))
		return;

	if (displayScreen(screen[1], 100.0, 560.0))
		return;
	
	if (displayScreen(screen[2], 100.0, 360.0))
		return;
}
