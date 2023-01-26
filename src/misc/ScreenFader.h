#ifndef _SCREENFADER_H
#define _SCREENFADER_H

/***********************************************/

#include "../Classes.h"
#include "../engine/GLColor.h"

/***********************************************/

class ScreenFader
{
	private:
		static GLColor currentColor;
	
	public:
	
		static void fadeToColor(GLColor color, double time);
};

/***********************************************/

#endif
