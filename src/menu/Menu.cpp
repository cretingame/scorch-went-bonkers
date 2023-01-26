#include <GL/gl.h>

#include "../menu/MenuOption.h"
#include "../misc/Settings.h"
#include "../misc/KeyReader.h"
#include "../sound/SoundSystem.h"

#include "Menu.h"

/**********************************************/

bool		Menu::autoCycle = false;
double	Menu::acCounter = 0.0;

/**********************************************/

Menu::Menu(double x, double y)
	: Entity(x, y)
{
	selected = -1;
	parentMenu = focusedMenu = menuToFocus = NULL;
	
	chosen = -1;
	
	totalHeight = 0.0;
	
	animProgress = 0.0;
	animState = AS_INVISIBLE;
	inTime = outTime = 0.1;
}

Menu::~Menu()
{
	for (unsigned int i = 0; i < options.size(); i++)
		delete options[i];
}

/**********************************************/

void Menu::addOption(MenuOption *o, Effect in, Effect out)
{
	options.push_back(o);
	optionIns.push_back(in);
	optionOuts.push_back(out);
	
	o->setParentMenu(this);
	
	if (in.time + in.delay > inTime)
		inTime = in.time + in.delay;
	if (out.time + out.delay > outTime)
		outTime = out.time + out.delay;
	
	totalHeight += o->getMarginTop() + o->getMarginBottom();
	
	if ((selected == -1) && (o->isSelectible()))
		setSelection(options.size() - 1);
}


void Menu::addOption(MenuOption *o)
{
	if (options.size() % 2)
	{
		Effect in(options.size() * 4.0, 30.0, E_LEFT);
		Effect out(options.size() * 4.0, 30.0, E_ENLARGE);
		addOption(o, in, out);
	}
	else
	{
		Effect in(options.size() * 4.0, 30.0, E_RIGHT);
		Effect out(options.size() * 4.0, 30.0, E_ENLARGE);
		addOption(o, in, out);
	}
}


void Menu::clearOptions()
{
	options.clear();
	optionIns.clear();
	optionOuts.clear();
	
	selected = -1;
}

/**********************************************/

void Menu::giveFocus(Menu *menu)
{
	menuToFocus = menu;
	if (menuToFocus)
		this->animFadeTo(AS_INVISIBLE);
}

void Menu::restoreFocus()
{
	menuToFocus = NULL;
	focusedMenu->animFadeTo(AS_INVISIBLE);
}

void Menu::immediateSetFocus(Menu *menu)
{
	focusedMenu = menu;
	menuToFocus = menu;
}

/**********************************************/

void Menu::selectAbove()
{
	if (selected == -1)
		return;
	
	options[selected]->deselect(MDIR_UP);
	
	do
	{
		selected--;
		if (selected < 0)
			selected = options.size() - 1;
	} while(options[selected]->isSelectible() == false);

	options[selected]->select(MDIR_DOWN);
	
	soundSys->playSound(SND_MENU_MOVE, 128, 0.5, 1.0);
}

void Menu::selectBelow()
{
	if (selected == -1)
		return;
	
	options[selected]->deselect(MDIR_DOWN);
	
	do
	{
		selected++;
		if (selected >= (int)options.size())
			selected = 0;
	} while(options[selected]->isSelectible() == false);

	options[selected]->select(MDIR_UP);

	soundSys->playSound(SND_MENU_MOVE, 128, 0.5, 1.0);
}

void Menu::setSelection(int nr)
{
	if (!options[nr]->isSelectible())
		return;
	
	if (selected != -1)
		options[selected]->deselect(MDIR_NONE);
	
	selected = nr;
	options[selected]->select(MDIR_NONE);
}

/**********************************************/

int Menu::getFinalChosen()
{
	if (focusedMenu) 
	{
		return focusedMenu->getFinalChosen();
	}
	
	int ret = chosen;
	chosen = -1; 
	return ret;
}

void Menu::setFinalChosen(int optionValue)
{
	chosen = optionValue;
}

/**********************************************/

void Menu::render()
{
	if (focusedMenu)
	{
		focusedMenu->render();
		return;
	}
	
	if (animState == AS_INVISIBLE)
		return;
	
	glPushMatrix();
	
	glTranslatef(x, y - totalHeight / 2, 0.0);
	
	double itemProgress;
	for (unsigned int i = 0; i < options.size(); i++)
	{
		if (animState == AS_TRANS_TO_FINISHED)
			itemProgress = (animProgress - optionIns[i].delay) / optionIns[i].time;
		else
			itemProgress = (animProgress + optionOuts[i].delay - outTime + optionOuts[i].time) / optionOuts[i].time;
		
		if (itemProgress <= 0.0) 
		{
			glTranslatef(0.0, options[i]->getMarginTop() + options[i]->getMarginBottom(), 0.0);
			continue;
		}
		
		if (itemProgress > 1.0) itemProgress = 1.0;
		
		glTranslatef(0.0, options[i]->getMarginTop(), 0.0);
		glPushMatrix();
		
		EffectType eff = E_NO_EFFECT;
		switch (animState)
		{
			case AS_TRANS_TO_FINISHED:
				eff = optionIns[i].type;
				break;
			case AS_TRANS_TO_INVISIBLE:
				eff = optionOuts[i].type;
				break;
			default:
				break;
		}
		
		switch(eff)
		{
			case E_WILD:
				glScalef(itemProgress * itemProgress, itemProgress * itemProgress, 1.0);
				glTranslatef( (1.0 - (itemProgress * itemProgress)) * (-300.0), (1.0 - itemProgress) * (-200.0), 0.0);
				glRotatef(itemProgress * 360.0, 0.0, 0.0, 1.0);
				break;
			case E_ENLARGE:
				glScalef(itemProgress * itemProgress, itemProgress * itemProgress, 1.0);
				break;
			case E_LEFT:
				glTranslatef((1.0 - itemProgress) * (-800.0), 0.0, 0.0);
				break;
			case E_RIGHT:
				glTranslatef((1.0 - itemProgress) * 800.0, 0.0, 0.0);
				break;
			case E_TOP:
				glTranslatef(0.0, (1.0 - itemProgress) * (-600.0), 0.0);
				break;
			case E_BOTTOM:
				glTranslatef(0.0, (1.0 - itemProgress) * 600.0, 0.0);
				break;
			default:
				break;
		}
		
		options[i]->renderAt(0.0, 0.0);
		
		glPopMatrix();
		glTranslatef(0.0, options[i]->getMarginBottom(), 0.0);
	}
	
	glPopMatrix();
}

/**********************************************/

void Menu::animFadeTo(AnimState state)
{
	if (focusedMenu)
	{
		focusedMenu->animFadeTo(state);
		return;
	}
	
	if (state == AS_FINISHED)
	{
		animProgress = 0.0;
		animState = AS_TRANS_TO_FINISHED;
	}
	if (state == AS_INVISIBLE)
	{
		animProgress = outTime;
		animState = AS_TRANS_TO_INVISIBLE;
	}
	
	acCounter = 0.0;
}

void Menu::animJumpTo(AnimState state)
{
	if (focusedMenu)
	{
		focusedMenu->animJumpTo(state);
		return;
	}
	
	animState = state;
	acCounter = 0.0;
}

AnimState Menu::getAnimState()
{
	if (focusedMenu)
		return focusedMenu->getAnimState();

	return animState;
}

/**********************************************/

void Menu::checkKeys()
{
	static bool joyControl = true;
	
	bool select = false;
	int keyScan = KEY_ENTER;
	
	for(KEY_ITERATOR k = KeyReader::scancodes.begin(); k != KeyReader::scancodes.end(); k++)
	{
		int scancode = (*k);
		switch(scancode)
		{
			case KEY_UP:
				selectAbove();
				autoCycle = false;
				break;
			case KEY_DOWN:
				selectBelow();
				autoCycle = false;
				break;
			case KEY_C:
				acCounter = 0.0;
				autoCycle = true;
				break;
			case KEY_ESC:
				break;
			default:
				select = true;
				keyScan = scancode;
				break;
		}
	}
	
	if (mouse_b != 0)
		select = true;

	poll_joystick();
	for (int joyNr = 0; joyNr < num_joysticks; joyNr++)
	{
		if ((joy[joyNr].num_sticks > 0) && (joy[joyNr].stick[0].num_axis >= 1))
		{
			if (joy[joyNr].stick[0].axis[1].d2)
			{
				if (joyControl)
				{
					selectBelow();
					autoCycle = false;
				}
				joyControl = false;
			}
			else if (joy[joyNr].stick[0].axis[1].d1)
			{
				if (joyControl)
				{
					selectAbove();
					autoCycle = false;
				}
				joyControl = false;
			}
			else if (joy[joyNr].stick[0].axis[0].d1)
			{
				if (joyControl)
				{
					keyScan = KEY_LEFT;
					select = true;
				}
				joyControl = false;				
			}
			else if (joy[joyNr].stick[0].axis[0].d2)
			{
				if (joyControl)
				{
					keyScan = KEY_RIGHT;
					select = true;
				}
				joyControl = false;				
			}
			else
				joyControl = true;
		}
	}
	
	for (int joyNr = 0; joyNr < num_joysticks; joyNr++)
		for (int buttNr = 0; buttNr < joy[joyNr].num_buttons; buttNr++)
			if (joy[joyNr].button[buttNr].b)
				select = true;

	if (select && selected != -1)
	{
		if (keyScan == KEY_LEFT || keyScan == KEY_RIGHT)
			if (!options[selected]->respondsToLeftRight()) return;
		
		options[selected]->action(keyScan);
		soundSys->playSound(SND_MENU_SELECT, 128, 0.5, 1.0);
	}
}

/**********************************************/

void Menu::step(double dt)
{
	if (focusedMenu)
	{
		focusedMenu->step(dt);
		
		if ((menuToFocus == NULL) && (focusedMenu->getAnimState() == AS_INVISIBLE))
		{
			focusedMenu = NULL;
			this->animFadeTo(AS_FINISHED);
		}

		return;
	}

	if (autoCycle && animState == AS_FINISHED) acCounter += dt;
	if (acCounter >= 150.0 - Settings::setting[ACC_CYCLE_SPEED] * 25.0) 
	{
		selectBelow();
		acCounter = 0.0;
	}

	if (animState == AS_FINISHED)
		checkKeys();
	
	if (animState == AS_TRANS_TO_FINISHED)
	{
		animProgress += dt;
		if (animProgress > inTime)
		{
			animProgress = inTime;
			animState = AS_FINISHED;
		}
	}

	if (animState == AS_TRANS_TO_INVISIBLE)
	{
		animProgress -= dt;
		if (animProgress < 0.0)
		{
			animProgress = 0.0;
			animState = AS_INVISIBLE;
			
			if (menuToFocus)
			{
				focusedMenu = menuToFocus;
				focusedMenu->animFadeTo(AS_FINISHED);
			}
		}
	}
}
