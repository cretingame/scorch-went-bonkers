#ifndef _BACKGROUND_H
#define _BACKGROUND_H

/**********************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/**********************************************/

class Background
	: public Entity
{
	protected:
		
		Theme *theme;
	
	public:
		
		Background(Theme *_theme);
	
		int		type() {return ETYPE_BACKGROUND;};
		
		int		renderPriority() {return 100;};
		int		stepPriority() {return 51;};
		
		double	scrollingSpeed() {return 1.0;};
		
		void		step(double dt);
		void		render();
};

/**********************************************/

#endif
