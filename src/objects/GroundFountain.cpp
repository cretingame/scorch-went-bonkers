#include <list>

#include "../objects/GroundDebris.h"
#include "GroundFountain.h"

/****************************************************/

GroundFountain::GroundFountain(double x, double y, int count, double baseAngle, double power)
{
	for(int i = 0; i < count; i++)
		debris.push_back(new GroundDebris(x, y, baseAngle, power));
}

/****************************************************/

void GroundFountain::setParent(GameWorld *gw)
{
	Entity::setParent(gw);
	
	std::list<Entity*>::iterator it;
	for(it = debris.begin(); it != debris.end(); it++)
		(*it)->setParent(gw);
}

/****************************************************/

void GroundFountain::step(double dt)
{
	std::list<Entity*>::iterator it;
	
	for(it = debris.begin(); it != debris.end();)
	{
		(*it)->step(dt);
		if ((*it)->isAlive())
			it++;
		else
		{
			delete (*it);
			it = debris.erase(it);
		}
	}
}

/****************************************************/

void GroundFountain::render()
{
	std::list<Entity*>::iterator it;
	
	glColor3f(0.2, 0.2, 0.0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	float x, y;
	glBegin(GL_QUADS);
		for(it = debris.begin(); it != debris.end(); it++)
		{
			x = (*it)->getX(); y = (*it)->getY();
			glVertex2f(x, y); glVertex2f(x+1.2, y);
			glVertex2f(x+1.2, y+1.2); glVertex2f(x, y+1.2);
		}
	glEnd();
}
