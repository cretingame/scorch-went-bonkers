#ifndef _MENU_H
#define _MENU_H

/******************************************/

#include <vector>

#include <allegro.h>

#include "../entity/Entity.h"

using namespace std;

/******************************************/

enum EffectType {E_NO_EFFECT, E_BLEND, E_LEFT, E_RIGHT, E_TOP, E_BOTTOM, E_ENLARGE, E_WILD};

struct Effect
{
	double	delay, time;
	EffectType	type;
	
	Effect(double _delay, double _time, EffectType _type)
		: delay(_delay), time(_time), type(_type) {};
};

/******************************************/

enum AnimState {AS_INVISIBLE, AS_TRANS_TO_FINISHED, AS_TRANS_TO_INVISIBLE, AS_FINISHED};

/******************************************/

class Menu
	: public Entity
{
	protected:
		
		Menu *parentMenu, *menuToFocus, *focusedMenu;
		int selected;
	
		vector<MenuOption*> options;
		vector<Effect> optionIns, optionOuts;
		double animProgress, inTime, outTime;
		AnimState animState;

		int		chosen;
	
		double	totalHeight;
	
		static bool		autoCycle;
		static double	acCounter;
	
	public:
		
		Menu(double _x, double _y);
		virtual ~Menu();
	
		void		giveFocus(Menu *menu);
		void		restoreFocus();
		void		immediateSetFocus(Menu *menu);
	
		void		setSelection(int nr);
		void		selectAbove();
		void		selectBelow();

		Menu*		getParentMenu() {return parentMenu;};
		void		setParentMenu(Menu *menu) {parentMenu = menu;};
	
		virtual void	addOption(MenuOption *option, Effect in, Effect out);
		virtual void	addOption(MenuOption *option);
		virtual void	clearOptions();
	
		int		getFinalChosen();
		void		setFinalChosen(int optionValue);
		
		void		animFadeTo(AnimState targetState);
		void		animJumpTo(AnimState targetState);
		AnimState	getAnimState();
		
		int		type() {return ETYPE_MENU;};
		int		renderPriority() {return 0;};
		int		stepPriority() {return 100;};
		
		virtual void	checkKeys();
		
		void		step(double dt);
		void		render();
};

/******************************************/

#endif
