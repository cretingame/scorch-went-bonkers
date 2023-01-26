#ifndef _TEXTFLOAT_H
#define _TEXTFLOAT_H

/**********************************************/

#include <string>

#include "../Classes.h"
#include "../entity/Entity.h"
#include "../engine/GLColor.h"

using namespace std;

/**********************************************/

class TextFloat
	: public Entity
{
	protected:
		
		double		opacity;
		string		text;
		GLColor		color;
		double		size;
	
	public:
		TextFloat(double _x, double _y, const char *text, GLColor color, double _size = 0.25);
	
		int		renderPriority()		{return 1;};
		int		stepPriority()		{return 1;};
		
		bool		isAlive()			{return opacity > 0.0;};
		
		void		step(double dt);
		void		render();
};

/**********************************************/

#endif
