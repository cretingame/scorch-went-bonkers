#ifndef _TARGETBOARD_H
#define _TARGETBOARD_H

/***************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/***************************************/

class TargetBoard
	: public Entity
{
	protected:
		int shotsHit, shotsFired;
	
	public:
		TargetBoard();
	
		void		addHit() {shotsHit++;};
		void		addFired() {shotsFired++;};
		
		int		getHits() {return shotsHit;};
		int		getFired() {return shotsFired;};
		double	getAccuracy() {return (shotsFired) ? (double)shotsHit * 100 / shotsFired : 0.0;};
		
		int		renderPriority() {return 1;};
		double	scrollingSpeed() {return -0.4;};
		void		render();
};

/***************************************/

#endif
