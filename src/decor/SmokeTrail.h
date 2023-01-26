#ifndef _SMOKETRAIL_H
#define _SMOKETRAIL_H

/****************************************/

#include "../Classes.h"
#include "../decor/Trail.h"
#include "../engine/GLBitmap.h"

/****************************************/

class SmokeTrail
	: public Trail
{
	protected:
		
		static GLBitmap		*smokeBMP;

		GLColor		color;
	
	public:
		SmokeTrail(Player *owner, Entity *target, double _interval, double _persistence);

		static void		loadBitmaps() {smokeBMP = GLBitmap::load("gfx/bullets/smoke.tga"); smokeBMP->setOrigin(0.5, 0.5);};
		static void		freeBitmaps() {delete smokeBMP;};

		void    changeOwner(Player *newOwner);
		void    renderTrail(TrailInfo t);
};

/****************************************/

#endif
