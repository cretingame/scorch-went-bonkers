#include "Entity.h"

/***************************************************************/

bool stepPriGT(Entity *e1, Entity *e2)
{
	return e1->stepPriority() > e2->stepPriority();
};


bool collisionPriGT(Entity *e1, Entity *e2)
{
	return e1->collisionPriority() > e2->collisionPriority();
};


bool renderPriGT(Entity *e1, Entity *e2)
{
	return e1->renderPriority() > e2->renderPriority();
};
