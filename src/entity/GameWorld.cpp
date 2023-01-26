#define EL_ITERATOR list<Entity*>::iterator

/***********************************************/

#include <list>
#include <algorithm>

#include "../engine/GLColor.h"
#include "../control/KeyControl.h"
#include "../control/MouseControl.h"
#include "../objects/Terrain.h"
#include "../objects/Tank.h"
#include "../objects/Background.h"
#include "../misc/Random.h"
#include "../misc/Settings.h"
#include "../entity/Entity.h"
#include "../entity/Sorters.h"

#include "GameWorld.h"

using namespace std;

/************************************************/

GameWorld::GameWorld()
{
	pauseState = PS_PLAYING;
	
	physics.gravity = 0.002 + 0.0004 * Settings::setting[GPL_GRAVITY];
	physics.airFriction = 0.00025 * Settings::setting[GPL_AIR_FRICTION];
	physics.wind = 0.0;
}

GameWorld::~GameWorld()
{
	EL_ITERATOR it;
	
	for (it = autoDestroyList.begin(); it != autoDestroyList.end(); it++)
		delete *it;
}

/************************************************/

void GameWorld::insertIntoList(list<Entity*>& elist, Entity *e, bool (*greaterFunc)(Entity*, Entity*) )
{
	EL_ITERATOR it;
	
	for (it = elist.begin(); it != elist.end(); it++)
		if (greaterFunc(e, (*it)))
			break;
	
	elist.insert(it, e);
}

/************************************************/

void GameWorld::add(Entity *e, bool autoDestroy)
{
	masterList.push_back(e);
	
	if (e->type() != ETYPE_UNKNOWN)
		typedList.push_back(e);
	
	if (autoDestroy)
		autoDestroyList.push_back(e);
	
	e->setParent(this);
	
	if (e->isArea())
		areas.push_back(e);

	if (e->renderPriority() >= 0)
		insertIntoList(renderSorted, e, renderPriGT);
	if (e->stepPriority() >= 0)
		insertIntoList(stepSorted, e, stepPriGT);
	if (e->collisionPriority() >= 0)
		insertIntoList(colSorted, e, collisionPriGT);
}

/************************************************/

void GameWorld::remove(Entity *e)
{
	masterList.remove(e);
	
	if (e->type() != ETYPE_UNKNOWN)
		typedList.remove(e);
	
	e->setParent(NULL);
	
	if (e->renderPriority() >= 0)
		renderSorted.remove(e);
	if (e->stepPriority() >= 0)
		stepSorted.remove(e);
	if (e->collisionPriority() >= 0)
		colSorted.remove(e);
	if (e->isArea())
		areas.remove(e);
	
	if (find(autoDestroyList.begin(), autoDestroyList.end(), e) != autoDestroyList.end())
	{
		delete e;
		autoDestroyList.remove(e);
	}
}

/*************************************************/

list<Entity*> GameWorld::getEntities()
{
	return masterList;
}

list<Entity*> GameWorld::getEntitiesOfType(int type)
{
	EL_ITERATOR it;
	list<Entity*> newList;
	
	for (it = typedList.begin(); it != typedList.end(); it++)
		if ((*it)->type() == type)
			newList.push_back(*it);
	
	return newList;
}

int GameWorld::countEntitiesOfType(int type)
{
	int count = 0;
	EL_ITERATOR it;
	
	for (it = typedList.begin(); it != typedList.end(); it++)
		if ((*it)->type() == type)
			count++;
	
	return count;
}

/*************************************************/

void GameWorld::setPauseImmediate(int ps)
{
	pauseState = PauseState(ps);
	
	if (pauseState == PS_PLAYING)
		pauseTransition = 0.0;
	if (pauseState == PS_PAUSED)
		pauseTransition = 1.0;
}

void GameWorld::togglePause()
{
	switch(pauseState)
	{
		case PS_PLAYING:
		case PS_TRANS_TO_PLAYING:
			pauseState = PS_TRANS_TO_PAUSED;
			break;
		
		case PS_PAUSED:
		case PS_TRANS_TO_PAUSED:
			pauseState = PS_TRANS_TO_PLAYING;
			break;
	}
}

/*************************************************/

void GameWorld::processRender()
{
	if (pauseState == PS_PAUSED) return;
		
	//glPushMatrix();
	
	EL_ITERATOR it;
	for (it = renderSorted.begin(); it != renderSorted.end(); it++)
	{
		if (pauseState != PS_PLAYING)
			glTranslatef(0.0, pauseTransition * 600.0 * (*it)->scrollingSpeed(), 0.0);
		
		(*it)->render();
		
		if (pauseState != PS_PLAYING)
			glTranslatef(0.0, -pauseTransition * 600.0 * (*it)->scrollingSpeed(), 0.0);
	}
	
	//glPopMatrix();
}

/*************************************************/

void GameWorld::processStep(double dt)
{
	EL_ITERATOR it;
	
	if (pauseState == PS_TRANS_TO_PLAYING)
	{
		pauseTransition -= dt * 0.005;
		if (pauseTransition <= 0.0)
		{
			pauseTransition = 0.0;
			pauseState = PS_PLAYING;
		}
	}
	if (pauseState == PS_TRANS_TO_PAUSED)
	{
		pauseTransition += dt * 0.005;
		if (pauseTransition >= 1.0)
		{
			pauseTransition = 1.0;
			pauseState = PS_PAUSED;
		}
	}

	for (it = stepSorted.begin(); it != stepSorted.end(); it++)
	{
		if ((pauseState == PS_PLAYING) || ((*it)->type() == ETYPE_MENU)) {
			if ((*it)->isAlive())
			{
				(*it)->step(dt);
				
				if (!(*it)->isAlive())
					removeQueue.push_back(*it);
			}
			else
				removeQueue.push_back(*it);
		}
	}
	
	for (it = removeQueue.begin(); it != removeQueue.end(); it++)
	{
		remove(*it);
	}
	
	
	removeQueue.clear();
}

/*************************************************/

void GameWorld::processCollisions()
{
	EL_ITERATOR objIt, areaIt;
	
	for (objIt = colSorted.begin(); objIt != colSorted.end(); objIt++)
	{
		for (areaIt = areas.begin(); areaIt != areas.end(); areaIt++)
		{
			if ((*areaIt)->contains((*objIt)->getX(), (*objIt)->getY()))
			{
				(*areaIt)->processCollision(*areaIt, *objIt);
				(*objIt)->processCollision(*areaIt, *objIt);
			}
		}
	}
}

