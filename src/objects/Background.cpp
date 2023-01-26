#include "../decor/Cloud.h"
#include "../entity/GameWorld.h"
#include "../engine/GLBitmap.h"
#include "../misc/Random.h"
#include "../misc/Theme.h"

#include "Background.h"

/************************************************/

Background::Background(Theme *_theme)
	: Entity(0.0, 0.0), theme(_theme)
{}

/************************************************/

void Background::step(double dt)
{
	list<Entity*> clouds = parent->getEntitiesOfType(ETYPE_CLOUD);
	list<Entity*>::iterator it;
	
	double probability = 0.0;
	for (it = clouds.begin(); it != clouds.end(); it++)
		probability += (250.0 - (*it)->getX()) / 2500.0;
	
	probability *= (double)theme->getMaxClouds() / (double)clouds.size();
	
	if (randomFloat(0.0, 1.0) < probability)
		parent->add(new Cloud(theme, randomInt(0, theme->getCloudsTypeCount() - 1), true), true);
}

/************************************************/

void Background::render()
{
	theme->getBackImage()->draw((int)x, (int)y);
}
