#ifndef _GROUNDFOUNTAIN_H
#define _GROUNDFOUNTAIN_H

/*****************************************/

#include <list>

#include "../Classes.h"
#include "../entity/Entity.h"

/*****************************************/

class GroundFountain : public Entity
{
	private:
		std::list<Entity*> debris;
		
	public:
		GroundFountain(double x, double y, int count, double baseAngle, double power);
		
		void	setParent(GameWorld *gw);
		
		bool	isAlive() {return !debris.empty();};
		
		int	stepPriority() {return 0;};
		int	renderPriority() {return 10;};
		
		void	step(double dt);
		void	render();
};

/*****************************************/

#endif
