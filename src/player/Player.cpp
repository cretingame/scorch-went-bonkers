#include "../objects/Tank.h"
#include "../control/TankControl.h"
#include "Player.h"

/*****************************************/

Player::Player(const char *_name, GLColor _color, TankControl *_controller)
	: name(_name), color(_color), controller(_controller)
{
	score = 0;
	currentTank = NULL;
}


Player::~Player()
{
	if (currentTank)
		delete currentTank;
	if (controller)
		delete controller;
}

/*****************************************/

Tank *Player::getCurrentTank()
{
	return currentTank;
}

Tank *Player::getNewTank()
{
	if (currentTank)
		delete currentTank;
	
	currentTank = new Tank(this, 0.0, 0.0);
	controller->setControlled(currentTank);
	currentTank->setController(controller);
	
	return currentTank;
}
