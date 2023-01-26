#include "../decor/SmokeTrail.h"
#include "../engine/GLBitmap.h"
#include "../entity/Airborne.h"
#include "../entity/GameWorld.h"
#include "../objects/GroundFountain.h"
#include "../objects/Explosion.h"
#include "../objects/Terrain.h"
#include "../misc/Settings.h"
#include "../sound/SoundSystem.h"
#include "Bullet.h"

/*****************************************/

GLBitmap *Bullet::bulletBMP = NULL;
GLBitmap *Bullet::markerBMP = NULL;

/*****************************************/

void Bullet::loadBitmaps()
{
	bulletBMP = GLBitmap::load("gfx/bullets/bullet.tga", GLBitmap::TF_MAGIC_PINK);
	bulletBMP->setOrigin(0.5, 0.5);
	
	markerBMP = GLBitmap::load("gfx/bullets/marker.tga", GLBitmap::TF_MAGIC_PINK);
	markerBMP->setOrigin(0.5, 0.0);
}

void Bullet::freeBitmaps()
{
	delete bulletBMP;
	delete markerBMP;
}

/*****************************************/

void Bullet::step(double dt)
{
	Airborne::step(dt);
	
	if (x < 0)
	{
		x = -x;
		vx = -vx;
	}
	if (x > 800)
	{
		x = 1600 - x;
		vx = -vx;
	}

	if (y >= 600)
		aliveFlag = false;
}

/******************************************/

void Bullet::spawnTrail()
{
    if (Settings::setting[GFX_SMOKE] > 0)
    {
        smokeTrail = new SmokeTrail(owner, this, 6.0 / Settings::setting[GFX_SMOKE], 90.0);
        parent->add(smokeTrail, true);
    }
}

/******************************************/

void Bullet::changeOwner(Player *newOwner)
{
	this->owner = newOwner;
	if (smokeTrail)
	    smokeTrail->changeOwner(newOwner);
}

/******************************************/

void Bullet::render()
{
	int drawX = (int)(x + 0.5);
	int drawY = (int)(y + 0.5);
	
	if (drawY > -1)
		bulletBMP->draw(x, y);
	else
		markerBMP->draw(drawX, 1.0); 
}

/******************************************/

void Bullet::processCollision(Entity *area, Entity *collider)
{
	if ((area->type() == ETYPE_EXPLOSION) || (area->type() == ETYPE_SHIELD))
		return;
	
	aliveFlag = false;
	parent->add(new Explosion(owner, x, y, Settings::setting[GPL_BLAST_RADIUS] * 9.0 + 15.0, 9.0, 100.0, 90.0), true);
	
	if (area->type() == ETYPE_TERRAIN)
	{
		Terrain *terr = (Terrain*)area;
		parent->add(new GroundFountain(x, y, Settings::setting[GFX_DEBRIS] * 30, terr->getNormalAngleAt(x), 0.3), true);
	}

	soundSys->playSound(SND_BOOM, (int)((x - 400.0) * 256.0 / 400.0) + 128, 0.2);
}
