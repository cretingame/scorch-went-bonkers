#ifndef _PLAYER_H
#define _PLAYER_H

/*********************************************/

#include "../Classes.h"
#include "../engine/GLColor.h"

#include <string>

using namespace std;

/*********************************************/

class Player{
	
	protected:
		
		string		name;
		GLColor		color;
		TankControl*	controller;
		Tank*			currentTank;
		int			score;
	
	public:
		
		Player(const char *name, GLColor color, TankControl *controller);
		~Player();
	
		string		getName() {return name;};
		GLColor		getColor() {return color;};
		void			setColor(GLColor _color) {color = _color;};
		TankControl*	getController() {return controller;};
		void			setController(TankControl *_controller) {controller = _controller;};
		int				getScore() {return score;};
		void			setScore(int _score) {score = _score;};
		
		void			addScore(int howMuch) {score += howMuch;};
		
		Tank*			getNewTank();
		Tank*			getCurrentTank();
};

/*********************************************/

#endif
