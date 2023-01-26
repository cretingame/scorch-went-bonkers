#ifndef _TARGET_H
#define _TARGET_H

/***************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/***************************************/

class Target
	: public Entity
{
	protected:
	
		bool		hit;
	
		double	animProgress, animTime;
		int		animDir;
	
		static GLBitmap		*targetBMP;
	
	public:
		Target(double _x, GameWorld *where, double _animTime);
	
		static void		loadBitmaps();
		static void		freeBitmaps();
	
		int		type()			{return ETYPE_TARGET;};
		
		double	getAnimProgress()		{return animProgress;};
		
		int		collisionPriority()	{return 10;};
		int		stepPriority()		{return 50;};
		int		renderPriority()		{return 50;};
		
		bool		isAlive()			{return (!hit) || (animProgress > 0.0);};
		bool		isHit()			{return hit;};

		
		bool		isArea()			{return true;};
		bool		contains(double cx, double cy);
		
		bool		isDamagable()		{return true;};
		void		doDamage(double dmg, Player *damageSource);
		
		void		step(double dt);
		void		render();
};

/***************************************/

#endif
