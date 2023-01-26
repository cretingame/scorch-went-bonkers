#include <algorithm>
#include <allegro.h>

#include "../engine/GLBitmap.h"
#include "../sound/SoundSystem.h"
#include "../misc/Random.h"
#include "../misc/Settings.h"

#include "Explosion.h"


using namespace std;

/**********************************************/

GLBitmap	*Explosion::circleBMP = NULL;
GLBitmap	*Explosion::flameBMP  = NULL;

/**********************************************/

Explosion::Explosion(Player *_owner, double _x, double _y, double _maxRadius, double _maxDmgRadius, double _maxDamage, double _totalTime, GLColor _color)
	: Entity(_x, _y), owner(_owner), maxRadius(_maxRadius), maxDmgRadius(_maxDmgRadius), 
	  maxDamage(_maxDamage), totalTime(_totalTime), color(_color)
{
	curTime = 0.0;
	curRadius = 0.0;
	curDamage = maxDamage;

	flameCount = ((int)(maxRadius / 4.0)) * Settings::setting[GFX_EXPLOSION];
	
	for (int i = 0; i < flameCount; i++)
	{
		flameDirX[i] = randomFloat(-0.7, 0.7);
		flameDirY[i] = randomFloat(-0.7, 0.7);
		flameSize[i] = randomFloat(0.8 / 30, 1.1 / 30);
		flameRotation[i] = randomFloat(0.0, 20.0);
		flameTransp[i] = (i * 5 / 2 / flameCount == 0) ? GLBitmap::TM_ALPHA_BLEND : GLBitmap::TM_ALPHA_ADDITIVE;
	}
}

/**********************************************/

void Explosion::loadBitmaps()
{
	circleBMP = GLBitmap::load("gfx/explosions/circle.tga");
	circleBMP->setOrigin(0.5, 0.5);
	circleBMP->setTint(GLColor(1.0, 0.3, 0.0));
	

	flameBMP = GLBitmap::load("gfx/explosions/flame.tga");
	flameBMP->setOrigin(0.5, 0.5);
	flameBMP->setTransparencyMode(GLBitmap::TM_ALPHA_BLEND);
}


void Explosion::freeBitmaps()
{
	delete circleBMP;
	delete flameBMP;
}

/**********************************************/

void Explosion::recalculateThings()
{
	double done = curTime / totalTime;
	if (done > 1.0) done = 1.0;
		
	curRadius = done * maxRadius;
	curDamage = (curRadius < maxDmgRadius) ? maxDamage : maxDamage * (1.0 - (curRadius - maxDmgRadius) / (maxRadius - maxDmgRadius)); 
}

/**********************************************/

void Explosion::step(double dt)
{
	double done = curTime / totalTime;
	curTime += dt * (1.0 - done * 0.7);
	recalculateThings();
}

/**********************************************/

void Explosion::render()
{
	double done = curTime / totalTime;
	double flameAlpha = (done > 0.5) ? (0.8 - (done - 0.5) * 1.6) : 0.8;
	double circleAlpha = Settings::setting[GFX_EXPLOSION] > 0 ? 0.3 : 0.7;
	
	circleBMP->setScale(curRadius / 32.0, curRadius / 32.0);
	circleBMP->setTint(GLColor(color.r, color.g, color.b, (float)(circleAlpha - done * circleAlpha)));
	if (Settings::setting[GFX_EXPLOSION] > 0)
		circleBMP->setTransparencyMode(GLBitmap::TM_ALPHA_ADDITIVE);
	else
		circleBMP->setTransparencyMode(GLBitmap::TM_ALPHA_BLEND);
	circleBMP->draw(x, y);
	
	if (Settings::setting[GFX_EXPLOSION] > 0)
	{
		for (int i = 0; i < flameCount; i++)
		{
			flameBMP->setScale(flameSize[i] * curRadius, flameSize[i] * curRadius);
			flameBMP->setRotation(flameRotation[i] * curRadius);
			flameBMP->setTransparencyMode(flameTransp[i]);		
				
			flameBMP->setTint(GLColor(
				(float)(color.r * flameAlpha),
				(float)(color.g * flameAlpha),
				(float)(color.b * flameAlpha),
				(float)flameAlpha
			));
				
			flameBMP->draw(x + flameDirX[i] * curRadius, y + flameDirY[i] * curRadius);
		}
	}
}

/**********************************************/

bool Explosion::contains(double cx, double cy)
{
	double diffX = cx - x;
	double diffY = cy - y;
	
	if (diffX * diffX + diffY * diffY > curRadius * curRadius)
		return false;
	else
		return true;
}

void Explosion::processCollision(Entity *area, Entity *collider)
{
	if (curDamage <= 0.0) return;
	if (!collider->isDamagable()) return;
	
	list<Entity*>::iterator it = find(alreadyDamaged.begin(), alreadyDamaged.end(), collider);
	if (it != alreadyDamaged.end())
		return;
	
	alreadyDamaged.push_back(collider);
	collider->doDamage(curDamage, owner);
}
