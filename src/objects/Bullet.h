#ifndef _BULLET_H
#define _BULLET_H

#include <allegro.h>

#include "../engine/GLBitmap.h"
#include "../entity/Airborne.h"
#include "../Classes.h"

/*****************************************/

class Bullet
	: public Airborne
{
	private:
		
		static GLBitmap *bulletBMP, *markerBMP;

		Player*		    owner;
		SmokeTrail*     smokeTrail;
		bool		    aliveFlag;

	public:

		static void loadBitmaps();
		static void freeBitmaps();
		
		Bullet(Player *_owner, double _x, double _y, double _vx, double _vy)
			: Airborne(_x, _y, _vx, _vy, 1.0), owner(_owner) { aliveFlag = true; smokeTrail = NULL; };
	
		void	changeOwner(Player *newOwner);
		Player*	getOwner() { return owner; };

		int	type() {return ETYPE_BULLET;};
		int	collisionPriority() {return 50;};
		int	stepPriority() {return 55;};
		int	renderPriority() {return 40;};

		void    spawnTrail();

		void	step(double dt);
		void	render();
		bool	isAlive() {return aliveFlag;};
		
		void	processCollision(Entity *area, Entity *collider);
	
};

/*****************************************/

#endif
