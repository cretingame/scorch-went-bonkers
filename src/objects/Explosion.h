#ifndef _EXPLOSION_H
#define _EXPLOSION_H

/*****************************************/

#include <list>

#include "../Classes.h"
#include "../entity/Entity.h"
#include "../engine/GLBitmap.h"

using namespace std;

/*****************************************/

const int XPL_FLAMES = 60;
const GLColor COLOR_NORMAL = GLColor(1.0, 1.0, 1.0);

class Explosion
	: public Entity
{
	private:
		
		Player *owner;

		int flameCount;
	
		double curRadius, maxRadius, maxDmgRadius;
		double curDamage, maxDamage;
		double curTime, totalTime;
	
		GLColor		color;
	
		double flameDirX[XPL_FLAMES], flameDirY[XPL_FLAMES], flameRotation[XPL_FLAMES], flameSize[XPL_FLAMES];
		GLBitmap::TransparencyMode flameTransp[XPL_FLAMES];

		list<Entity*> alreadyDamaged;

		static	GLBitmap *circleBMP, *flameBMP;

		void		recalculateThings();

	public:
		Explosion(Player *_owner, double _x, double _y, double _maxRadius, double _maxDmgRadius, double _maxDamage, double _totalTime, GLColor color = COLOR_NORMAL);

		static void		loadBitmaps();
		static void		freeBitmaps();
	
		int		type() {return ETYPE_EXPLOSION;};
		
		int		stepPriority() {return 60;};
		int		renderPriority() {return 45;};
		bool		isAlive() {return curTime < totalTime;};
		
		void		step(double dt);
		void		render();
		
		bool		isArea() {return true;};
		bool		contains(double x, double y);
		void		processCollision(Entity *area, Entity *collider);
		
};

/*****************************************/

#endif
