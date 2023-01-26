#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "../engine/TextRenderer.h"
#include "../entity/GameWorld.h"
#include "../control/TankControl.h"
#include "../decor/SmokeTrail.h"
#include "../decor/TextFloat.h"
#include "../misc/Random.h"
#include "../misc/TextHandler.h"
#include "../misc/Settings.h"
#include "../player/Player.h"
#include "../objects/Explosion.h"
#include "../objects/Shield.h"
#include "../objects/Bullet.h"
#include "../objects/GroundFountain.h"
#include "../objects/Terrain.h"
#include "../sound/SoundSystem.h"

#include "Tank.h"

/***************************************************************/

GLBitmap	*Tank::bodyBMP = NULL;
GLBitmap	*Tank::gunBMP = NULL;
GLBitmap	*Tank::crosshairBMP = NULL;
GLBitmap	*Tank::respawnBMP = NULL;
int			Tank::count = 0;

/***************************************************************/

Tank::Tank(Player *_owner, double _x, double _y)
	: Entity(_x, _y), owner(_owner), color(_owner->getColor())
{
	shieldColor = GLColor(color.r / 2 + 0.5, color.g / 2 + 0.5, color.b / 2 + 0.5);

	gunAngle = randomFloat(20.0, 160.0); gunPower = 0.0;
	gunDirection = 1;
	state = TS_IDLE;
	health = 100.0;
	cooldownCounter = 0.0;
	controller = NULL;
	invincible = false;

	shield = NULL;
	
	if (count == 0)
	{
		bodyBMP = GLBitmap::load("gfx/tanks/body.tga", GLBitmap::TF_MAGIC_PINK);
		gunBMP = GLBitmap::load("gfx/tanks/gun.tga", GLBitmap::TF_NO_ALPHA);
		crosshairBMP = GLBitmap::load("gfx/tanks/crosshair.tga", GLBitmap::TF_MAGIC_PINK);

		bodyBMP->setOrigin(0.5, 0.5);
		
		gunBMP->setOrigin(0.5, 1.0);
		gunBMP->setTransparencyMode(GLBitmap::TM_ALPHA_BLEND);
		
		crosshairBMP->setTransparencyMode(GLBitmap::TM_ALPHA_CUT);
		crosshairBMP->setOrigin(0.5, 0.5);
	}
	
	count++;
}


Tank::~Tank()
{
	count--;

	if (count == 0)
	{
		delete bodyBMP; delete gunBMP; delete crosshairBMP;
	}		
}

/***************************************************************/

bool Tank::contains(double cx, double cy)
{
	// no collision states
	if ((state == TS_FALLING) || (state == TS_RESPAWNING))
		return false;

	// test
	if ( (cy >= y + 0.5) && ( cy <= y + 4.5) && (cx >= x - 12) && (cx <= x + 12) )
		return true;
	
	if ( (cy >= y - 3.5) && ( cy <= y + 0.5) && (cx >= x - 5) && (cx <= x + 5) )
		return true;
	
	return false;
}

/***************************************************************/

bool Tank::isAlive() {
	return health > 0.0;
}

/***************************************************************/

void Tank::applyBehavior(TankBehavior whatToDo, double dt) {
	switch (whatToDo)
	{
		case TB_SHIELD:
			if (shield == NULL)
			{
				if (Settings::setting[GPL_SHIELD_TIME] > 0)
				{
					gunPower = 0.0;
					shield = new Shield(owner, x, y, shieldColor, Settings::setting[GPL_SHIELD_TIME] * 20.0);
					parent->add(shield, false);
					if ((state == TS_SHOOTING) || (state == TS_SLOW_SHOOTING))
						state = TS_IDLE;
				}
				else
				{
					gunPower = 0.0;
					state = TS_COOLDOWN;
					cooldownCounter = dt;
				}
			}
			break;
			
		case TB_SLOW_SHOOTING:
			if (state == TS_IDLE)
				state = TS_SLOW_SHOOTING;
			break;
			
		case TB_START_SHOOTING:
			if (state == TS_IDLE || state == TS_SLOW_SHOOTING)
				state = TS_SHOOTING;
			break;
			
		case TB_SHOOT:
			if (state == TS_SHOOTING)
			{
				if (parent)
				{
					double vx = std::cos(gunAngle * M_PI / 180) * gunPower * 2;
					double vy = -std::sin(gunAngle * M_PI / 180) * gunPower * 2;
					double shotX = x + vx * 10 / gunPower;
					double shotY = y + vy * 10 / gunPower;
					Bullet *shot = new Bullet(owner, shotX, shotY, vx, vy);
					parent->add(shot, true);
					shot->spawnTrail();
				}
				
				if (gunAngle > 90.0)
					gunDirection = 1;
				else
					gunDirection = -1;
				
				state = TS_COOLDOWN;
				cooldownCounter = 100.0 * Settings::setting[GPL_DOWN_TIME];
				
				gunPower = 0.0;
				
				soundSys->playSound(SND_SHOOT, (int)((x - 400.0) * 256.0 / 400.0) + 128, 0.2);
			}
			break;
		default:
			break;
	}
}

void Tank::step(double dt)
{
	// allow for control
	if (controller)
	{
		TankBehavior whatToDo = controller->getBehavior(dt);

		if ((state != TS_RESPAWNING) && (state != TS_FALLING))
			applyBehavior(whatToDo, dt);		
	}

	// keep track of the shield
	if (shield && (!shield->isAlive()) && (!shield->getParent()))
	{
		delete shield;
		shield = NULL;
	}
	
	// sway the barrel in idle
	if (state == TS_IDLE)
	{
		gunAngle += gunDirection * dt * 0.5;
		if (gunAngle > 160.0)
		{
			gunDirection = -1;
			gunAngle = 160.0 - (gunAngle - 160.0);
		}
		if (gunAngle < 20.0)
		{
			gunDirection = 1;
			gunAngle = 20.0 + (20.0 - gunAngle);
		}
	}
	
	// charge the gun when shooting (or slow shooting?)
	if (state == TS_SHOOTING)
	{
		if (gunPower < 1.5)
			gunPower += 0.005 * dt;
	};
	
	if (state == TS_SLOW_SHOOTING)
	{
		if (gunPower < 1.5)
			gunPower += 0.001 * dt;
	}
	
	// bring the gun down in cooldown, and wait for it to end
	if (state == TS_COOLDOWN)
	{
		gunAngle += gunDirection * dt * 0.2;

		if (gunAngle > 160.0)
			gunAngle = 160.0;
		if (gunAngle < 20.0)
			gunAngle = 20.0;

		cooldownCounter -= dt;
		if (cooldownCounter <= 0.0)
		{
			gunDirection = -gunDirection;
			state = TS_IDLE;
		}
	}

	// when falling, fall
	if (state == TS_FALLING)
	{
		Terrain *terrain = (Terrain*)parent->getEntitiesOfType(ETYPE_TERRAIN).front();
		double terrainHeight = terrain->getYAt(x);
		y += 4.0 * dt;
		if (y > terrainHeight) {
			y = terrainHeight;
			state = TS_IDLE;
		}
	}

	// when respawning, count down
	if (state == TS_RESPAWNING)
	{
		respawnCounter -= dt;
		if (respawnCounter <= 0.0)
			state = TS_FALLING;
	}
}

/***************************************/

void Tank::render()
{
	// only marker when respawning
	if (state == TS_RESPAWNING) {
		bodyBMP->setTint(color);
		bodyBMP->draw(x, y);
		return;
	}

	// otherwise, render the whole tank
	gunBMP->setTint(color);
	gunBMP->setRotation(gunAngle - 90.0);
	gunBMP->draw(x, y);
	
	bodyBMP->setTint(color);
	bodyBMP->draw(x, y);
	
	if (state != TS_COOLDOWN)
	{
		double chX, chY;
		chX = (int)(x + std::cos(gunAngle * M_PI / 180) * (60.0 - gunPower * 30.0)) + 0.5;
		chY = (int)(y - std::sin(gunAngle * M_PI / 180) * (60.0 - gunPower * 30.0)) + 0.5;
		crosshairBMP->setTint(color);
		crosshairBMP->draw(chX, chY);
	}
	
	if (health > 0.0)
	{
		GLColor healthColor;
		if (health > 50.0)
		{
			healthColor.r = (100.0 - health) / 50.0;
			healthColor.g = ((health - 50.0) * 0.6 + (100 - health) * 1.0) / 50.0;
		}
		else
		{
			healthColor.r = 1.0;
			healthColor.g = health / 50.0;
		}
		healthColor.b = 0.0;
		
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(x - 10, y + 8); glVertex2f(x + 10, y + 8);
			glVertex2f(x + 10, y + 12); glVertex2f(x - 10, y + 12);
			glColor3f(healthColor.r, healthColor.g, healthColor.b);
			glVertex2f(x - 9, y + 9); glVertex2f(x - 9 + 18 * health / 100, y + 9);
			glVertex2f(x - 9 + 18 * health / 100, y + 11); glVertex2f(x - 9, y + 11);
		glEnd();
	}
}

/********************************************/

void Tank::doDamage(double dmg, Player *damageSource)
{
	if (health <= 0.0) return;
	if (invincible) return;
		

	if (shield) {
		// defend from damage
		dmg *= 1.0 - shield->effectiveness() * 0.8;
	}

	health -= dmg;
	lastDamagedBy = damageSource;
	
	char buf[8];
	sprintf(buf, "%d", (int)dmg);
	
	parent->add(new TextFloat(x, y, buf, GLColor(1.0, 0.5, 0.5), 0.32), true);
	
	if (health <= 0.0)
	{
		parent->add(new Explosion(NULL, x, y, 60.0, 0.0, 0.0, 400.0), true);		
		soundSys->playSound(SND_BOOM, (int)((x - 400.0) * 256.0 / 400.0) + 128, 0.2);
	}
}

void Tank::respawn(GameWorld *world, double x, double y)
{
	lastDamagedBy = NULL;
	health = 100.0;
	state = TS_RESPAWNING;
	respawnCounter = 300.0;

	setX(x); setY(2.0);

	if (parent == NULL)
		world->add(this, false);
}