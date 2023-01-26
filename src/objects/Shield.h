#ifndef _SHIELD_H
#define _SHIELD_H

/*********************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

#include <list>

using namespace std;

/*********************************************/

class Shield
	: public Entity
{
	protected:
		
		static GLBitmap*		shieldBMP;
	
		Player*			owner;
		GLColor			color;
		double			passedTime, totalTime;
	
		list<Entity*>	alreadyDeflected;
	
	public:
		Shield(Player *owner, double _x, double _y, GLColor _color, double _totalTime);
	
		static void		loadBitmaps();
		static void		freeBitmaps();
	
		int		type() {return ETYPE_SHIELD;};
		int		renderPriority() {return 40;};
		int		stepPriority() {return 40;};
		bool		isAlive() {return passedTime < totalTime;};
		
		double 		effectiveness();
	
		bool		isArea() {return true;};
		bool		contains(double cx, double cy);
		
		void		processCollision(Entity *area, Entity *collider);
		
		void		step(double dt);
		void		render();
};

/*********************************************/

#endif
