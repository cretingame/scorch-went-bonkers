#include <cstdio>
#include <GL/gl.h>
#include <alleggl.h>

#include "menu/Menu.h"
#include "menu/MenuOption.h"
#include "menu/FinalOption.h"
#include "menu/AscenderOption.h"
#include "menu/ChoiceOption.h"
#include "menu/DescenderOption.h"
#include "menu/MultiOption.h"
#include "menu/StaticText.h"
#include "menu/StaticImage.h"
#include "menu/KeySelectMenu.h"

#include "engine/TimeKeeper.h"
#include "engine/GLBitmap.h"
#include "misc/KeyReader.h"
#include "misc/Settings.h"
#include "misc/ScreenFader.h"
#include "misc/Quitting.h"
#include "game/BattleGame.h"
#include "game/TargetGame.h"
#include "game/FragGame.h"
#include "player/Player.h"
#include "control/KeyControl.h"
#include "control/MouseControl.h"
#include "control/DroneControl.h"

#include "sound/SoundSystem.h"

#include "MainMenu.h"

enum OptionCode {OP_BATTLE_MODE = 1, OP_TARGET_MODE, OP_FRAG_MODE, OP_EXIT};

/**********************************************/

ChoiceOption *compPlyOption[5];

/**********************************************/

// Callback for the music setting
void musicCallback(MenuOption *)
{
	soundSys->setMusicVolume(100);
}

/**********************************************/

MainMenu::MainMenu()
{
	TK = new TimeKeeper(0.01, 0.025, 2, true);

	/** DETAILS MENU **/
	mDetails = new Menu(400, 300);
	
	mDetails->addOption(new StaticText("Graphics", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	mDetails->addOption(new MultiOption("Display mode", &Settings::setting[GFX_FULLSCREEN], "Windowed,Fullscreen"));
	mDetails->addOption(new StaticText("changing this option will have effect only after restarting the program", GLColor(0.9, 0.9, 0.9),
		7.0, 25.0, 0.25));
	mDetails->addOption(new MultiOption("Clouds", &Settings::setting[GFX_CLOUDS], "Off,On"));
	mDetails->addOption(new MultiOption("Explosions", &Settings::setting[GFX_EXPLOSION], "Basic,Simple,Normal,Cute"));
	mDetails->addOption(new MultiOption("Debris", &Settings::setting[GFX_DEBRIS], "None,Little,Enough,Loads"));
	mDetails->addOption(new MultiOption("Shell smoke", &Settings::setting[GFX_SMOKE], "Disabled,Low detail,Medium detail,Hi detail"));
	mDetails->addOption(new AscenderOption("Back", 25.0));
	
	/** GAMEPLAY MENU **/
	mGameplay = new Menu(400, 300);
	mGameplay->addOption(new StaticText("Gameplay", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	mGameplay->addOption(new MultiOption("Game speed", &Settings::setting[GPL_GAME_SPEED], "Very slow,Slow,Normal,Fast,Insane", 15.0, 25.0));
	mGameplay->addOption(new MultiOption("Gravity", &Settings::setting[GPL_GRAVITY], "Weaker,Weak,Standard,Strong,Stronger"));
	mGameplay->addOption(new MultiOption("Air friction", &Settings::setting[GPL_AIR_FRICTION], "No friction,Weak,Standard,Strong", 15.0, 25.0));
	mGameplay->addOption(new MultiOption("Shield lasting time", &Settings::setting[GPL_SHIELD_TIME], "No shields, Brief, Normal, Long"));
	mGameplay->addOption(new MultiOption("Recharge time", &Settings::setting[GPL_DOWN_TIME], "No recharge,Brief,Normal,Long,Irritating"));
	mGameplay->addOption(new MultiOption("Blast radius", &Settings::setting[GPL_BLAST_RADIUS], "Small,Normal,Large,Huge"));
	
	mGameplay->addOption(new AscenderOption("Back", 25.0));
	
	/** SOUND MENU **/
	mSound = new Menu(400, 300);
	mSound->addOption(new StaticText("Sound", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
		
	mSound->addOption(new MultiOption("Menu music", &Settings::setting[SND_MUSIC], "Off,On", 15.0, 15.0, musicCallback, musicCallback));
	mSound->addOption(new MultiOption("Sound effects", &Settings::setting[SND_SFX], "Off,On"));
	
	mSound->addOption(new AscenderOption("Back", 25.0));
	
	/** ACCESS MENU **/
	mAccess = new Menu(400, 300);
	mAccess->addOption(new StaticText("Accessibility", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	mAccess->addOption(new MultiOption("Menu cycle speed", &Settings::setting[ACC_CYCLE_SPEED], "Slower,Slow,Normal,Fast,Faster"));
	mAccess->addOption(new MultiOption("Ingame tap threshold", &Settings::setting[ACC_TAP_SPEED], "Normal,Long,Longer,Longest"));
	mAccess->addOption(new MultiOption("Delay shooting", &Settings::setting[ACC_SLOW_SHOOTING], "No,Yes"));
	
	mAccess->addOption(new AscenderOption("Back", 25.0));
	
	/** OPTIONS MENU **/
	mOptions = new Menu(400, 300);
	
	mOptions->addOption(new StaticText("Options", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
		
	mOptions->addOption(new DescenderOption("Accessibility", mAccess));
	mOptions->addOption(new DescenderOption("Graphics", mDetails));
	mOptions->addOption(new DescenderOption("Gameplay", mGameplay));
	mOptions->addOption(new DescenderOption("Sound", mSound));
	
	mOptions->addOption(new AscenderOption("Back", 25.0));
	
	/** BATTLE MENU **/
	mBattle = new Menu(400, 300);
	mBattle->addOption(new StaticText("King of the Hill", GLColor(1.0, 1.0, 0.0), 0.0, 20.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	mBattle->addOption(new StaticText("last man standing scores a point", GLColor(0.9, 0.9, 0.9),
		7.0, 38.0, 0.3));
	
	mBattle->addOption(new MultiOption("Number of players", &this->rawPlayerCount, "Two,Three,Four,Five"));
	mBattle->addOption(new MultiOption("Computer players", &this->rawCompPlayerCount, "Zero,One,Two,Three,Four", 25.0, 15.0));
	mBattle->addOption(new MultiOption("Computer skill", &this->rawCompSkill, "Moron,Level 1,Level 2,Level 3,Level 4,Level 5,Marksman,Sharpshooter,Cyborg", 15.0, 25.0));
	mBattle->addOption(new MultiOption("Rounds to win", &this->rawRoundCount, "One,Two,Three,Five,Ten,Twenty", 15.0, 25.0));
	
	mBattle->addOption(new FinalOption("Start", OP_BATTLE_MODE));
	mBattle->addOption(new AscenderOption("Back"));
	
	/** FRAG MENU **/
	mFrag = new Menu(400, 300);
	mFrag->addOption(new StaticText("Frag Battle", GLColor(1.0, 1.0, 0.0), 0.0, 20.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	mFrag->addOption(new StaticText("first to a given number of frags wins", GLColor(0.9, 0.9, 0.9),
		7.0, 38.0, 0.3));

	mFrag->addOption(new MultiOption("Number of players", &this->rawPlayerCount, "Two,Three,Four,Five"));
	mFrag->addOption(new MultiOption("Computer players", &this->rawCompPlayerCount, "Zero,One,Two,Three,Four", 25.0, 15.0));
	mFrag->addOption(new MultiOption("Computer skill", &this->rawCompSkill, "Moron,Level 1,Level 2,Level 3,Level 4,Level 5,Marksman,Sharpshooter,Cyborg", 15.0, 25.0));
	mFrag->addOption(new MultiOption("Total kills needed", &this->rawFragCount, "One,Three,Five,Ten,Twenty,Thirty,Fifty", 15.0, 25.0));
	
	mFrag->addOption(new FinalOption("Start", OP_FRAG_MODE));
	mFrag->addOption(new AscenderOption("Back"));
	
	/** TARGET MENU **/
	mTarget = new Menu(400, 300);
	mTarget->addOption(new StaticText("Target Practice", GLColor(1.0, 1.0, 0.0), 0.0, 20.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	mTarget->addOption(new StaticText("practice alone against random targets", GLColor(0.9, 0.9, 0.9),
		7.0, 38.0, 0.3));
	
	mTarget->addOption(new MultiOption("Starting time", &this->rawPlayTime, "15 seconds,30 seconds,60 seconds,90 seconds,120 seconds,Free play"));
	mTarget->addOption(new MultiOption("Bonus per target", &this->rawTimeBonus, "1 second,2 seconds,4 seconds,6 seconds,8 seconds,10 seconds,15 seconds,30 seconds", 15.0, 25.0));
	
	mTarget->addOption(new FinalOption("Start", OP_TARGET_MODE));
	mTarget->addOption(new AscenderOption("Back"));
		
	/** MAIN MENU **/
	
	GLBitmap *logoBig = GLBitmap::load("gfx/logo/big.tga");
	GLBitmap *logoSmall = GLBitmap::load("gfx/logo/small.tga");
	
	mMain = new Menu(400, 300);
	
	mMain->addOption(new StaticImage(logoBig, 74.0, 74.0),
		Effect(10.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	mMain->addOption(new StaticImage(logoSmall, 30.0, 100.0),
		Effect( 0.0, 30.0, E_WILD), Effect(4.0, 30.0, E_ENLARGE));
	
	mMain->addOption(new StaticText("Main menu", GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6), 
		Effect(20.0, 30.0, E_BOTTOM), Effect(8.0, 30.0, E_ENLARGE));
	
	mMain->addOption(new DescenderOption("Frag Battle", mFrag));	
	mMain->addOption(new DescenderOption("King of the Hill", mBattle));
	mMain->addOption(new DescenderOption("Target Practice", mTarget, 15.0, 25.0));
	mMain->addOption(new DescenderOption("Options", mOptions));
	mMain->addOption(new FinalOption("Exit", OP_EXIT, 25.0, 80.0));
	
	/** SETTINGS **/
	rawPlayerCount = 0;
	rawRoundCount = 1;
	rawFragCount = 2;
	rawCompPlayerCount = 0;
	rawCompSkill = 2;
	
	rawPlayTime = 1;
	rawTimeBonus = 3;
}

MainMenu::~MainMenu()
{
	delete mMain;
}

/**********************************************/

void MainMenu::assignController(Player *player)
{
	KeySelectMenu *mKeySelect;
	char buf[200];
	double delta;
	
	mKeySelect = new KeySelectMenu(400, 300);
	
	sprintf(buf, "%s player", player->getName().c_str());
	mKeySelect->addOption(new StaticText(buf, player->getColor(), 0.0, 30.0, 0.6),
		Effect(0.0, 30.0, E_ENLARGE), Effect(0.0, 30.0, E_ENLARGE));
	mKeySelect->addOption(new StaticText("Please press the button you want to use.", GLColor(1.0, 1.0, 1.0)),
		Effect(4.0, 30.0, E_ENLARGE), Effect(4.0, 30.0, E_ENLARGE));
	mKeySelect->animFadeTo(AS_FINISHED);

	TK->reset();
	while((mKeySelect->getAnimState() != AS_INVISIBLE) || (mKeySelect->getController() == NULL))
	{
		delta = TK->getDT() * 100; // we want delta in 0.01s
		do 
		{
			// logic
			KeyReader::update();

			mKeySelect->step(delta);
			
			if (mKeySelect->getController() && mKeySelect->getAnimState() == AS_FINISHED)
				mKeySelect->animFadeTo(AS_INVISIBLE);
			
		}while(TK->getFramesToSkip() > 0);

		// drawing
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		
		mKeySelect->render();
		allegro_gl_flip();
	}
	
	player->setController(mKeySelect->getController());
	delete mKeySelect;
}

/**********************************************/

void MainMenu::playBattleGame()
{
	static const double computerSkillLUT[] = {1.0, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.02};
	Player *players[5];
	BattleGame *game;
	
	int roundCount = 1;
	switch(rawRoundCount)
	{
		case 0: case 1: case 2: roundCount = rawRoundCount + 1; break;
		case 3: roundCount = 5; break;
		case 4: roundCount = 10; break;
		case 5: roundCount = 20; break;
	}
	int playerCount = rawPlayerCount + 2;
	int compPlayers = (rawCompPlayerCount > playerCount - 1) ? playerCount - 1 : rawCompPlayerCount;
	int humanPlayers = playerCount - compPlayers;
	
	players[0] = new Player("Red", GLColor(1.0, 0.0, 0.0), NULL);
	players[1] = new Player("Yellow", GLColor(1.0, 1.0, 0.0), NULL);
	players[2] = new Player("Green", GLColor(0.0, 1.0, 0.0), NULL);
	players[3] = new Player("Blue", GLColor(0.0, 0.4, 1.0), NULL);
	players[4] = new Player("White", GLColor(1.0, 1.0, 1.0), NULL);

	/** CONTROLLER SELECTION **/
	
	for (int i = 0; i < humanPlayers; i++)
		assignController(players[i]);
	
	/** COMPUTER PLAYERS **/
	
	for (int i = humanPlayers; i < playerCount; i++)
		players[i]->setController(new DroneControl(NULL, computerSkillLUT[rawCompSkill]));
	
	/** REST OF THE STUFF **/
	
	game = new BattleGame(players, playerCount, roundCount);
	game->play();
	
	for (int i = playerCount; i < 5; i++)
		delete players[i];
	
	delete game;
}

/**********************************************/

void MainMenu::playTargetGame()
{
	static const double playTimeLUT[] =  {15.0, 30.0, 60.0, 90.0, 120.0, 30000.0};
	static const double timeBonusLUT[] = {1.0, 2.0, 4.0, 6.0, 8.0, 10.0, 15.0, 30.0};
	
	Player *players[1];
	TargetGame *game;
	
	players[0] = new Player("Red", GLColor(1.0, 0.0, 0.0), NULL);
	assignController(players[0]);
	
	game = new TargetGame(players, playTimeLUT[rawPlayTime], timeBonusLUT[rawTimeBonus]);
	game->play();
	
	delete game;
}

/**********************************************/

void MainMenu::playFragGame()
{
	static const double computerSkillLUT[] = {1.0, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.02};
	static const int fragCountLUT[] = {1, 3, 5, 10, 20, 30, 50};
	Player *players[5];
	FragGame *game;
	
	int fragCount = fragCountLUT[rawFragCount];
	int playerCount = rawPlayerCount + 2;
	int compPlayers = (rawCompPlayerCount > playerCount - 1) ? playerCount - 1 : rawCompPlayerCount;
	int humanPlayers = playerCount - compPlayers;
	
	players[0] = new Player("Red", GLColor(1.0, 0.0, 0.0), NULL);
	players[1] = new Player("Yellow", GLColor(1.0, 1.0, 0.0), NULL);
	players[2] = new Player("Green", GLColor(0.0, 1.0, 0.0), NULL);
	players[3] = new Player("Blue", GLColor(0.0, 0.4, 1.0), NULL);
	players[4] = new Player("White", GLColor(1.0, 1.0, 1.0), NULL);

	/** CONTROLLER SELECTION **/
	
	for (int i = 0; i < humanPlayers; i++)
		assignController(players[i]);
	
	/** COMPUTER PLAYERS **/
	
	for (int i = humanPlayers; i < playerCount; i++)
		players[i]->setController(new DroneControl(NULL, computerSkillLUT[rawCompSkill]));
	
	/** REST OF THE STUFF **/
	
	game = new FragGame(players, playerCount, fragCount);
	game->play();
	
	for (int i = playerCount; i < 5; i++)
		delete players[i];
	
	delete game;
}

/**********************************************/

void MainMenu::go()
{
	double delta;
	int chosen = 0;
	bool somethingSelected;
	bool end = false;
	
	while(!end)
	{
		ScreenFader::fadeToColor(GLColor(55, 154, 255), 15.0);

		TK->reset();
		somethingSelected = false;
		
		mMain->immediateSetFocus(NULL);
		mOptions->immediateSetFocus(NULL);
		
		mMain->animFadeTo(AS_FINISHED);
		
		while((!somethingSelected) || (mMain->getAnimState() != AS_INVISIBLE))
		{			
			// close button handling
			if (Quitting::applicationQuitting()) {
				chosen = OP_EXIT;
				break;
			}

			delta = TK->getDT() * 100; // we want delta in 0.01s
			do 
			{
				// logic
				mMain->step(delta);
				
				int tempChosen = mMain->getFinalChosen();
				if (tempChosen >= 0)
				{
					chosen = tempChosen;
					somethingSelected = true;
					mMain->animFadeTo(AS_INVISIBLE);
				}
				
				KeyReader::update();
				
			}while(TK->getFramesToSkip() > 0);
	
			// drawing
			glClear(GL_COLOR_BUFFER_BIT);
			glLoadIdentity();
			
			mMain->render();
			allegro_gl_flip();
		}
		
		switch(chosen)
		{
			case OP_BATTLE_MODE:
				playBattleGame();
				break;
			
			case OP_TARGET_MODE:
				playTargetGame();
				break;

			case OP_FRAG_MODE:
				playFragGame();
				break;
			
			case OP_EXIT:
				end = true;
				break;
		}
	}
}
