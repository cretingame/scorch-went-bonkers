#include <cmath>
#include <algorithm>

#include "../engine/GLBitmap.h"
#include "../objects/Bullet.h"
#include "../sound/SoundSystem.h"

#include "Shield.h"

/*********************************************/

GLBitmap *Shield::shieldBMP = NULL;

const double SHIELD_RADIUS = 13.5;

/*********************************************/

Shield::Shield(Player *_owner, double _x, double _y, GLColor _color, double _totalTime)
	: Entity(_x, _y), owner(_owner), color(_color), totalTime(_totalTime)
{
	passedTime = 1.0;
	
	soundSys->playSound(SND_SHIELD, (int)((x - 400.0) * 256.0 / 400.0) + 128, 0.5);
}

/*********************************************/

void Shield::loadBitmaps()
{
	shieldBMP = GLBitmap::load("gfx/tanks/shield.tga");
	shieldBMP->setOrigin(0.5, 0.5);
}

void Shield::freeBitmaps()
{
	delete shieldBMP;
}

/*********************************************/

double Shield::effectiveness()
{
	return 1.0 - (std::abs(totalTime / 2 - passedTime) / totalTime * 2);
}

/*********************************************/

bool Shield::contains(double cx, double cy)
{
	double distSquared = (cx - x) * (cx - x) + (cy - y) * (cy - y);
	
	if (distSquared < SHIELD_RADIUS * SHIELD_RADIUS)
		return true;
	
	return false;
}

/*********************************************/

void Shield::processCollision(Entity *area, Entity *collider)
{
	if (collider->type() != ETYPE_BULLET) return;
	if (area != this) return;
	if (std::find(alreadyDeflected.begin(), alreadyDeflected.end(), collider) != alreadyDeflected.end()) return;
	
	Bullet *bullet = (Bullet*)collider;
	
	double vx = bullet->getVX();
	double vy = bullet->getVY();
	double travelAngle = std::atan2(-vy, vx);
	double hitX = bullet->getX() - x;
	double hitY = bullet->getY() - y;
	double hitAngle = std::atan2(-hitY, hitX);
	double speed = std::sqrt(vx * vx + vy * vy);
	
	double newAngle = M_PI + 2 * hitAngle - travelAngle;	
	double speedModifier = effectiveness() < 0.1 ? 0.0 : 1.3;

	double newSpeed = speedModifier * speed;
	double newVX = std::cos(newAngle) * newSpeed;
	double newVY = -std::sin(newAngle) * newSpeed;
	
	bullet->setVX(newVX); bullet->setVY(newVY);
	bullet->changeOwner(owner);

	soundSys->playSound(SND_BOUNCE, (int)((bullet->getX() - 400.0) * 256.0 / 400.0) + 128, 0.5);

	alreadyDeflected.push_back(bullet);
}

/*********************************************/

void Shield::step(double dt)
{
	passedTime += dt;
}

/*********************************************/

void Shield::render()
{
	double eff = effectiveness();
	color.a = eff;
	
	shieldBMP->setTint(color);
	shieldBMP->draw((int)x, (int)y);
}
