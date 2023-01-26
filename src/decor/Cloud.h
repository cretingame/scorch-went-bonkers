#ifndef _CLOUD_H
#define _CLOUD_H

/*******************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/*******************************************/

class Cloud
	: public Entity
{
	protected:
		
		Theme		*theme;
		int		typeNr;
		double		width, height;
		double		scale;
		
		bool		aliveFlag;
		
		double	vx;
	
	public:
		
		Cloud(Theme *_theme, int _typeNr, bool offRight);
	
		int type()			{return ETYPE_CLOUD;};
		
		int renderPriority()	{return 90;};
		int stepPriority()	{return 50;};
		bool isAlive()		{return aliveFlag;};
		
		double scrollingSpeed()	{return 1.05;};
		
		void		step(double dt);
		void		render();
};

/*******************************************/

#endif
