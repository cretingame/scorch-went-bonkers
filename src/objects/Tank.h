#ifndef _TANK_H
#define _TANK_H

#include "../Classes.h"

#include "../entity/Entity.h"
#include "../engine/GLColor.h"
#include "../engine/GLBitmap.h"
#include "../engine/TextRenderer.h"
#include "../control/TankControl.h"

/***********************************/

enum TankState{TS_RESPAWNING, TS_FALLING, TS_IDLE, TS_SHOOTING, TS_COOLDOWN, TS_SLOW_SHOOTING};

class Tank :
	public Entity
{
	private:
		
		Player *owner;

		static int count;
		static GLBitmap *bodyBMP, *gunBMP, *crosshairBMP, *respawnBMP;
		
		GLColor color, shieldColor, smokeColor;
	
		double		health;
		Player*		lastDamagedBy;

		Shield*		shield;
		
		int		state;
		double	cooldownCounter, respawnCounter;
		double	gunAngle, gunPower;
		int		gunDirection;
	
		bool		invincible;
	
		TankControl *controller;
	
	private:
		void		applyBehavior(TankBehavior behavior, double dt);

	public:
		
		Tank(Player *owner, double x, double y);
		~Tank();

		void			setController(TankControl *control) {controller = control;};
		TankControl*	getController() {return controller;};
		
		int		type() {return ETYPE_TANK;};
		
		double	getPower() {return gunPower;};
		double	getAngle() {return gunAngle;};
		int		getGunDir() {return gunDirection;};
		int		getState() {return state;};
		
		int		collisionPriority() {return 10;};
		int		stepPriority()   {return 50;};
		int		renderPriority() {return 50;};

		bool		isAlive();
		Player*		getLastDamagedBy() { return lastDamagedBy; };
		
		bool		isArea() {return true;};
		bool		contains(double x, double y);
		
		bool		isDamagable() {return true;};
		void		doDamage(double dmg, Player *damageSource);
		void		setInvincible(bool _invincible) {invincible = _invincible;};
		
		void		respawn(GameWorld *world, double x, double y);

		void		render();
		void		step(double dt);
};

/***********************************/

#endif
