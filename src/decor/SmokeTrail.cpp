#include "../misc/Random.h"
#include "../engine/GLBitmap.h"
#include "../objects/Bullet.h"
#include "../player/Player.h"
#include "SmokeTrail.h"

/************************************************/

GLBitmap *SmokeTrail::smokeBMP = NULL;

/************************************************/

SmokeTrail::SmokeTrail(Player *_owner, Entity *_target, double _interval, double _persistence)
	: Trail(_target, _interval, _persistence)
{
    changeOwner(_owner);
}

/************************************************/

void SmokeTrail::changeOwner(Player *newOwner) {
	color = newOwner->getColor();
	color = GLColor(color.r * 0.8 + 0.2, color.g * 0.8 + 0.2, color.b * 0.8 + 0.2);
}

/************************************************/

void SmokeTrail::renderTrail(TrailInfo t)
{
	double done = t.time / persistence;
	double scale = 0.15 + 0.4 * done;
	GLColor drawColor;
	
	drawColor.a = 0.9 - done * 0.9;
	drawColor.r = color.r * (1.0 - done) + done;
	drawColor.g = color.g * (1.0 - done) + done;
	drawColor.b = color.b * (1.0 - done) + done;
	
	smokeBMP->setTransparencyMode(randomFloat(0.0, 1.0) > 0.5 ? GLBitmap::TM_ALPHA_BLEND : GLBitmap::TM_ALPHA_BLEND);

	smokeBMP->setTint(drawColor);
	
	smokeBMP->setScale(scale, scale);
	smokeBMP->setRotation(randomFloat(0.0, 360.0));
	
	smokeBMP->draw(t.x, t.y);
}
