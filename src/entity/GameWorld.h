#ifndef _GAMEWORLD_H
#define _GAMEWORLD_H

/***********************************************************/

#include <list>

#include "../Classes.h"
#include "../entity/Physics.h"

using namespace std;

/***********************************************************/

enum PauseState {PS_PLAYING, PS_PAUSED, PS_TRANS_TO_PAUSED, PS_TRANS_TO_PLAYING};
class GameWorld
{
	private:
		list <Entity*>	masterList, typedList, autoDestroyList, renderSorted, stepSorted, colSorted, areas;
		list <Entity*>	removeQueue;
	
		Tank		*tanks[3];
		Terrain		*terrain;
	
		Physics		physics;

		PauseState	pauseState;
		double		pauseTransition;

		void		insertIntoList(list<Entity*>& elist, Entity *e, bool (*greaterFunc)(Entity*, Entity*) );
	
	public:
		void		add(Entity *e, bool autoDestroy);
		void		remove(Entity *e);
	
		Physics&	getPhysics() {return physics;};

		int			getEntityCount() {return masterList.size();};
		list<Entity*>	getEntities();
		list<Entity*>	getEntitiesOfType(int type);
		int			countEntitiesOfType(int type);
		
		PauseState	getPauseState() {return pauseState;};
		double	getPauseTransition() {return pauseTransition;};
		void		setPauseImmediate(int ps);
		void		togglePause();
		
		void		processStep(double dt);
		void		processCollisions();
		void		processRender();
	
		GameWorld();
		~GameWorld();
};

/***********************************************************/

#endif
