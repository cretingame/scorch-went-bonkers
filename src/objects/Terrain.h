#ifndef _TERRAIN_H
#define _TERRAIN_H

/***************************************************/

#include "../entity/Entity.h"

/***************************************************/

const int TERRAIN_SURFACE_POINTS = 161;

class Terrain :
	public Entity
{
	private:
		
		static GLTexture *soilTxtr, *grassTxtr;

		double height[TERRAIN_SURFACE_POINTS];
	
	public:
		
		static void loadBitmaps();
		static void freeBitmaps();

		Terrain();

		void		flattenAt(double x);
		double		getHeightAt(double x);
		double		getYAt(double x);
		double		getNormalAngleAt(double x);

		int		type() {return ETYPE_TERRAIN;};
		int		renderPriority() {return 60;};
		
		bool		isArea() {return true;};
		bool		contains(double x, double y);
		
		void render();
};

/***************************************************/

#endif
