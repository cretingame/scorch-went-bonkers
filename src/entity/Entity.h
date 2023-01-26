#ifndef _ENTITY_H
#define _ENTITY_H

#include <allegro.h>
#include <list>

#include "../Classes.h"

using namespace std;

/***********************************************************/

const int ETYPE_UNKNOWN = -1;
const int ETYPE_TANK = 1;
const int ETYPE_BULLET = 2;
const int ETYPE_TERRAIN = 3;
const int ETYPE_EXPLOSION = 4;
const int ETYPE_SHIELD = 5;
const int ETYPE_TRAIL = 6;
const int ETYPE_TARGET = 7;
const int ETYPE_CLOUD = 8;

const int ETYPE_MENU = 255;
const int ETYPE_BACKGROUND = 254;

class Entity
{
	protected:
		GameWorld	*parent;
		double	x, y;
	
	public:
		virtual int		type() {return ETYPE_UNKNOWN;};
		
		GameWorld*		getParent() {return parent;};
		virtual void		setParent(GameWorld *world) {parent = world;};
		
		double		getX() {return x;};
		double		getY() {return y;};
		void			setX(double _x) {x = _x;};
		void			setY(double _y) {y = _y;};
		
		virtual int		collisionPriority() {return -1;};
		virtual int		stepPriority()	  {return -1;};
		virtual int		renderPriority()	  {return -1;};

		virtual void	step(double dt) {allegro_message("Ouch!");};
		virtual void	render() {};
		virtual bool	isAlive() {return true;};
		
		virtual void	processCollision(Entity *area, Entity *collider) {};
		
		virtual bool	isArea() {return false;};
		virtual bool	contains(double x, double y) {return false;};

		virtual bool	isDamagable() {return false;};
		virtual void	doDamage(double amount, Player *damageSource) {};
		
		virtual double	scrollingSpeed() {return 1.5;};
		
		Entity()				{parent = NULL;};
		Entity(double _x, double _y)	{x = _x; y = _y; parent = NULL;};
		
		virtual ~Entity() {};
};

/***********************************************************/

#endif
