#include <cstdarg>
#include <cstdio>
#include <ifstream>
#include <string>

#include <allegro.h>

#include "../engine/GLTexture.h"
#include "GLFont.h"

/**************************************************/

int GLFont::quality = QUALITY_HIGH;

/**************************************************/

GLFont::~GLFont()
{
	delete [] glyph;
	
	if (texture)
		delete texture;
}

/**************************************************/

GLFont *GLFont::load(std::string baseFileName)
{
	std::string texFName = baseFileName + ".pcx";
	std::string metFName = baseFileName + ".met";
	
	// read the metrics
	
	std::ifstream metrics(metFName);
	if (!metrics.good())
		allegro_message("Can't open font metrics: \n'%s'", metFName.c_str());
	
	metrics >> glyphCount;
	glyph = new GlyphData[glyphCount];
	
	for (int i = 0; i < glyphCount; i++)
		metrics >> glyph[i].x1 >> glyph[i].y1 >> glyph[i].x2 >> glyph[i].y2;
	
	metrics.close();
	
	// read the texture
	
	texture = GLTexture::load(texFName, GL_ALPHA, AGL_TEXTURE_ALPHA_ONLY);
	if (!texture)
		allegro_message("Can't open font texture: \n'%s'", texFName.c_str());
}

/**************************************************/

void GLFont::write(double x, double y, double hAlign, double vAlign, double size, GLColor color, std::string text)
{
	double width = textWidth(text, size);
	double height = textHeight(size);
	
	glPushMatrix();
	
	glTranslatef(x - hAlign * width, y - vAlign * height, 0.0);
	glScalef(size, size);
	color.set();
	texture->bind();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	double cursor = 0.0;
	double glyphHeight = glyph[0]->y2 - glyph[0]->y1;
	
	glBegin(GL_QUADS);
	for (int i = 0; i < text.size(); i++)
	{
		GlyphData *g = glyph[text[i] - 32];
		double glyphWidth = g->x2 - g->x1;
		
		glTexCoord2f(g->x1, g->y1); glVertex2f(cursor, 0.0);
		glTexCoord2f(g->x2, g->y1); glVertex2f(cursor + glyphWidth, 0.0);
		glTexCoord2f(g->x2, g->y2); glVertex2f(cursor + glyphWidth, glyphHeight);
		glTexCoord2f(g->x1, g->y2); glVertex2f(cursor, glyphHeight);
		
		cursor += glyphWidth;
	}
	glEnd();
	
	glPopMatrix();
};

/********************************************************/

double GLFont::textWidth(std::string text, double size)
{
	double totalWidth = 0.0;
	for (int i = 0; i < text.size; i++)
	{
		GlyphData *g = glyph[text[i] - 32];
		totalWidth += g->x2 - g->x1;
	}
	return totalWidth * size;
}

double GLFont::textHeight(double size)
{
	return (glyph[0]->y2 - glyph[0]->y1) * size;
}
