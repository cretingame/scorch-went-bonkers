#ifndef _TEXTRENDERER_H
#define _TEXTRENDERER_H

/***************************************************/

#include "../engine/GLColor.h"
#include "../engine/GLTexture.h"

/***************************************************/

struct GlyphData
{
	bool valid;
	double x1, y1, x2, y2;
};

/***************************************************/

class TextRenderer
{
	private:
		
		static char			commonBuffer[2000];
	
		GLTexture			*texture;
		GlyphData			glyphs[128];
	
		GLColor			color;
		double			sizeX, sizeY;
	
		TextRenderer();

	public:
		
		~TextRenderer();
		
		static TextRenderer* load(const char *bitmap, const char *metrics);
	
		void		setColor(GLColor color);
		void		setSize(double x, double y);
		
		void		render(double x, double y, double horizPos, double vertPos, const char *text);
		void		render(double x, double y, double horizPos, double vertPos, GLColor color, const char *text);
		void		printf(double x, double y, double horizPos, double vertPos, const char *format, ...);
		void		printf(double x, double y, double horizPos, double vertPos, GLColor color, const char *format, ...);
};

/***************************************************/

#endif
