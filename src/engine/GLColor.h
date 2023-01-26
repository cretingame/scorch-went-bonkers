#ifndef _GLCOLOR_H
#define _GLCOLOR_H

/************************************************/

#include <GL/gl.h>

/************************************************/

class GLColor
{
	public:
	
		float r, g, b, a;
		
		GLColor() {r = 0.0; g = 0.0; b = 0.0; a = 0.0;};
		GLColor(double _r, double _g, double _b, double _a = 1.0) {r = _r; g = _g; b = _b; a = _a;};
		GLColor(float _r, float _g, float _b, float _a = 1.0f) {r = _r; g = _g; b = _b; a = _a;};
		GLColor(int   _r, int   _g, int   _b, int   _a = 255)
		{
			r = ((float)_r) / 255.0f; g = ((float)_g) / 255.0f; b = ((float)_b) / 255.0f; a = ((float)_a) / 255.0f;
		}
		
		void set() {glColor4f(r, g, b, a);};
};

/************************************************/

#endif
