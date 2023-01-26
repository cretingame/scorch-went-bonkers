#include <allegro.h>
#include <alleggl.h>
#include <jpgalleg.h>

#include "player/Player.h"
#include "game/BattleGame.h"
#include "control/KeyControl.h"
#include "engine/GLColor.h"
#include "decor/SmokeTrail.h"

#include "objects/Bullet.h"
#include "objects/Explosion.h"
#include "objects/Terrain.h"
#include "objects/Shield.h"
#include "objects/Target.h"

#include "sound/SoundSystem.h"

#include "menu/SplashScreen.h"

#include "misc/Quitting.h"
#include "misc/TextHandler.h"
#include "misc/Random.h"
#include "misc/Settings.h"
#include "misc/ScreenFader.h"

#include "MainMenu.h"

/********************************************************/

void initAllegro()
{
	allegro_init();

	Settings::loadSettings();

	install_allegro_gl();
	
	allegro_gl_set(AGL_DOUBLEBUFFER, true);
	allegro_gl_set(AGL_COLOR_DEPTH, 32);
	allegro_gl_set(AGL_RENDERMETHOD, true);
	
	int suggested = AGL_DOUBLEBUFFER | AGL_COLOR_DEPTH | AGL_RENDERMETHOD;
	
	if (Settings::setting[GFX_FULLSCREEN])
	{
		allegro_gl_set(AGL_FULLSCREEN, true);
		suggested |= AGL_FULLSCREEN;
	}
	else
	{
		allegro_gl_set(AGL_WINDOWED, true);
		suggested |= AGL_WINDOWED;
	}
	
	allegro_gl_set(AGL_REQUIRE, suggested);
	
	set_color_depth(32);
	request_refresh_rate(100);
	
	if (set_gfx_mode(GFX_OPENGL, 800, 600, 0, 0) != 0)
	{
		allegro_gl_set(AGL_SUGGEST, suggested);
		if (set_gfx_mode(GFX_OPENGL, 800, 600, 0, 0) != 0)
		{
			allegro_message("Error setting up display: %s\nPlease adjust settings.ini and set a workable resolution.", allegro_error);
			exit(-1);
		}
	};
	
	rest(500);
	
	install_timer();
	install_keyboard();
	install_mouse();
	install_joystick(JOY_TYPE_AUTODETECT);
	
	jpgalleg_init();
}

/***************************************************/

void setUpGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, 5, -5);
	glMatrixMode(GL_MODELVIEW);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

/***************************************************/

void init()
{
	initAllegro();
	setUpGL();
	
	set_window_title("Scorch Went Bonkers");
	if (set_display_switch_mode(SWITCH_BACKGROUND) == -1)
		set_display_switch_mode(SWITCH_BACKAMNESIA);
	
	Quitting::registerHandler();
	
	initRandom();
	Shield::loadBitmaps();
	Bullet::loadBitmaps();
	Terrain::loadBitmaps();
	Explosion::loadBitmaps();
	SmokeTrail::loadBitmaps();
	Target::loadBitmaps();
	
	SoundSystem::loadAll();
	soundSys = new SoundSystem();
}

/***************************************************/

void destroy()
{
	Settings::saveSettings();
	
	Shield::freeBitmaps();
	Bullet::freeBitmaps();
	Terrain::freeBitmaps();
	Explosion::freeBitmaps();
	SmokeTrail::freeBitmaps();
	Target::freeBitmaps();

	delete soundSys;
	SoundSystem::freeAll();
	
	TextHandler::freeFonts();
}

/***************************************************/

int main()
{
	init();
	
	ScreenFader::fadeToColor(GLColor(55, 157, 255), 30.0);
	soundSys->playMusic(MOD_MENU, true, 0.4);
	
	SplashScreen *splashScreen = new SplashScreen();
	splashScreen->play();
	delete splashScreen;
	
	MainMenu *mainMenu = new MainMenu();
	mainMenu->go();
	delete mainMenu;
	
	destroy();
	
	return 0;
}
END_OF_MAIN();
