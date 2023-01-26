#include <GL/gl.h>
#include <alleggl.h>

#include "../engine/GLColor.h"
#include "../engine/TimeKeeper.h"

#include "ScreenFader.h"

/************************************************/

GLColor ScreenFader::currentColor = GLColor(0, 0, 0);

/************************************************/

void ScreenFader::fadeToColor(GLColor color, double time)
{
	TimeKeeper tk(0.01, 0.025, 2, true);
	
	GLColor actualColor;
	double transFactor = 0.0;

	tk.reset();
	while (transFactor < 1.0)
	{
		double delta = tk.getDT() * 100;
		
		transFactor += delta / time;
		if (transFactor > 1.0)
			transFactor = 1.0;
		
		actualColor.r = transFactor * color.r + (1.0 - transFactor) * currentColor.r;
		actualColor.g = transFactor * color.g + (1.0 - transFactor) * currentColor.g;
		actualColor.b = transFactor * color.b + (1.0 - transFactor) * currentColor.b;
		
		glClearColor(actualColor.r, actualColor.g, actualColor.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		allegro_gl_flip();
	}
	
	currentColor = color;
	glClearColor(currentColor.r, currentColor.g, currentColor.b, 1.0);
}
