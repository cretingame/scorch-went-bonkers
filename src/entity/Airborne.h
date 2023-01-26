#ifndef AIRBORNE_H
#define AIRBORNE_H

/*************************************************/

#include "../Classes.h"
#include "../entity/Entity.h"

/*************************************************/

class Airborne
	: public Entity
{
	protected:
		
		double vx, vy;
		double weight;
		
		double gravityAcc, fricIndex;
		
	public:
		Airborne(double _x, double _y, double _vx, double _vy, double _weight)
			: Entity(_x, _y), vx(_vx), vy(_vy), weight(_weight) {};
		virtual ~Airborne() {};
	
		double		getVX() {return vx;};
		double		getVY() {return vy;};
		double		getWeight() {return weight;};
		void		setVX(double _vx) {vx = _vx;};
		void		setVY(double _vy) {vy = _vy;};
		void		setWeight(double _weight) {weight = _weight;};
		
		void		setParent(GameWorld *gw);
		
		virtual void	step(double dt);
};

/*************************************************/

#endif
