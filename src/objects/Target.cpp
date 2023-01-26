#include <cmath>
#include <list>

#include "../engine/GLBitmap.h"
#include "../entity/GameWorld.h"
#include "../objects/Terrain.h"
#include "../objects/Explosion.h"

#include "Target.h"

using namespace std;

/************************************************/

GLBitmap	*Target::targetBMP = NULL;

/************************************************/

Target::Target(double _x, GameWorld *where, double _animTime)
	: Entity(_x, 0.0), animTime(_animTime)
{
	hit = false;
	animProgress = 0.0;
	animDir = 1;
	
	list<Entity*> l = where->getEntitiesOfType(ETYPE_TERRAIN);
	Terrain *terrain = (Terrain*)l.front();
	
	this->setY(590.0 - terrain->getHeightAt(this->getX()));
}

/************************************************/

void Target::loadBitmaps()
{
	targetBMP = GLBitmap::load("gfx/targets/target.tga", GLBitmap::TF_MAGIC_PINK);
	targetBMP->setOrigin(0.5, 0.5);
	targetBMP->setTransparencyMode(GLBitmap::TM_ALPHA_BLEND);
}

void Target::freeBitmaps()
{
	delete targetBMP;
}

/************************************************/

bool Target::contains(double cx, double cy)
{
	if ( std::abs(cx-x) < 5.5  &&  std::abs(cy-y+0.5) < 6.5)
		return true;
	
	return false;
}

/************************************************/

void Target::doDamage(double dmg, Player *damageSource)
{
	if (dmg > 60.0)
	{
		hit = true;
		animDir = -1;
	}
}

/************************************************/

void Target::step(double dt)
{
	if (animDir == 0) return;
		
	animProgress += animDir;
	if (animProgress > animTime)
	{
		animProgress = animTime;
		animDir = 0;
	}
	if (animProgress < 0.0)
	{
		animProgress = 0.0;
		animDir = 0;
	}
}

/************************************************/

void Target::render()
{
	int drawX = (int)x;
	int drawY = (int)y;
	
	targetBMP->setTint(GLColor(1.0, 1.0, 1.0, animProgress / animTime));
	targetBMP->draw(drawX, drawY + 0.5);
}
