#include <cstring>
#include <cstdio>
#include <stdarg.h>

#include <allegro.h>
#include <alleggl.h>
#include <GL/gl.h>

#include "GLColor.h"
#include "TextRenderer.h"

/***********************************************/

char TextRenderer::commonBuffer[2000];

/***********************************************/

TextRenderer::TextRenderer()
{
	sizeX = sizeY = 1.0;
	color = GLColor(1.0, 1.0, 1.0, 1.0);
}

/***********************************************/

TextRenderer::~TextRenderer()
{
	if (texture)
		delete texture;
}

/***********************************************/

TextRenderer* TextRenderer::load(const char *bitmapName, const char *metricsName)
{
	TextRenderer *ret = NULL;
	
	BITMAP *fontBMP = load_bitmap(bitmapName, NULL);
	
	if (!fontBMP)
	{
		allegro_message("Could not find font bitmap: %s", bitmapName);
		return ret;
	}
	
	bool needTemp = false;
	int newWidth = fontBMP->w, newHeight = fontBMP->h;
	
	if (fontBMP->w & (fontBMP->w - 1))
	{
		needTemp = true;
		newWidth = 1;
		while(newWidth < fontBMP->w) newWidth <<= 1;
	}
	if (fontBMP->h & (fontBMP->h - 1))
	{
		needTemp = true;
		newHeight = 1;
		while(newHeight < fontBMP->h) newHeight <<= 1;
	}
	
	if (newHeight > newWidth)
		newWidth = newHeight;
	if (newWidth > newHeight)
		newHeight = newWidth;
	
	GLTexture *tex;
	
	// actual texture creation part
	if (needTemp)
	{
		BITMAP *temp = create_bitmap_ex(bitmap_color_depth(fontBMP), newWidth, newHeight);
		
		//allegro_message("%d;%d", newWidth, newHeight);
		if (!temp)
		{
			allegro_message("Unable to create temporary bitmap");
			delete fontBMP;
			return NULL;
		}
		
		blit(fontBMP, temp, 0, 0, 0, 0, fontBMP->w, fontBMP->h);
		tex = GLTexture::create(temp, GL_ALPHA, AGL_TEXTURE_ALPHA_ONLY);
		destroy_bitmap(temp);
	}
	else
		tex = GLTexture::create(fontBMP, GL_ALPHA, AGL_TEXTURE_ALPHA_ONLY);

	if (!tex)
	{
		allegro_message("Unable to create font texture from %s", bitmapName);
		return ret;
	}

	destroy_bitmap(fontBMP);

	ret = new TextRenderer();
	ret->texture = tex;

	ret->texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	set_config_file(metricsName);

	for (int i = 0; i < 96; i++)
	{
		char buf[4];
		sprintf(buf, "%d", i);
		int width = get_config_int("chr", buf, 10);
		
		//allegro_message("Width: %d", width);
		
		if (width == 0)
			ret->glyphs[i+32].valid = false;
		else
		{
			ret->glyphs[i+32].valid = true;
			ret->glyphs[i+32].x1 = (i % 16) * 64.0 + 32.0 - width / 2;
			ret->glyphs[i+32].y1 = (i / 16) * 64.0 + 0.0;
			ret->glyphs[i+32].x2 = (i % 16) * 64.0 + 32.0 + (width + 1) / 2;
			ret->glyphs[i+32].y2 = (i / 16) * 64.0 + 63.9; 
			
			//allegro_message("(%.3lf,%.3lf):(%.3lf,%.3lf)", ret->glyphs[i+32].x1, ret->glyphs[i+32].y1, ret->glyphs[i+32].x2, ret->glyphs[i+32].y2);
			
			ret->glyphs[i+32].x1 /= 1024.0;
			ret->glyphs[i+32].x2 /= 1024.0;
			ret->glyphs[i+32].y1 /= 1024.0;
			ret->glyphs[i+32].y2 /= 1024.0;
		}
	}
	
	return ret;
}

/***********************************************/

void TextRenderer::setColor(GLColor _color)
{
	color = _color;
}


void TextRenderer::setSize(double x, double y)
{
	sizeX = x;
	sizeY = y;
}


/***********************************************/


void TextRenderer::render(double x, double y, double horizPos, double vertPos, const char *text)
{
	// quick hack
	vertPos += 0.1;
	
	glPushMatrix();
/*	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);*/
	
	glTranslatef(x, y, 0);
	glScalef(sizeX, sizeY, 1.0);
	
	double textWidth = 0.0;
	for(unsigned int i = 0; i < std::strlen(text); i++)
		textWidth += glyphs[(int)text[i]].x2 - glyphs[(int)text[i]].x1;
	
	glTranslatef(-horizPos * textWidth * 950.0, -vertPos * 0.0625 * 950.0, 0.0);
	
	color.set();
	texture->bind();
	
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	double cursorX = 0.0;
	glBegin(GL_QUADS);
	for(unsigned int i = 0; i < std::strlen(text); i++)
	{
		GlyphData *glyph = &glyphs[(int)text[i]];
		double glyphsWidth = (glyph->x2 - glyph->x1) * 950.0;
		glTexCoord2f(glyph->x1, glyph->y1); glVertex2f(cursorX, 0.0);
		glTexCoord2f(glyph->x2, glyph->y1); glVertex2f(cursorX + glyphsWidth, 0.0);
		glTexCoord2f(glyph->x2, glyph->y2); glVertex2f(cursorX + glyphsWidth, 59.375);
		glTexCoord2f(glyph->x1, glyph->y2); glVertex2f(cursorX, 59.375);
		cursorX += glyphsWidth;
	}
	glEnd();
	
/*	glPopAttrib();*/
	glPopMatrix();
}

void TextRenderer::render(double x, double y, double horizPos, double vertPos, GLColor color, const char *text)
{
	this->setColor(color);
	this->render(x, y, horizPos, vertPos, text);
}


/***********************************************/


void TextRenderer::printf(double x, double y, double horizPos, double vertPos, const char *format, ...)
{
	va_list varArg;
	
	va_start(varArg, format);
	vsprintf(commonBuffer, format, varArg);
	va_end(varArg);
	
	render(x, y, horizPos, vertPos, commonBuffer);
}

void TextRenderer::printf(double x, double y, double horizPos, double vertPos, GLColor color, const char *format, ...)
{
	va_list varArg;
	
	va_start(varArg, format);
	vsprintf(commonBuffer, format, varArg);
	va_end(varArg);
	
	this->setColor(color);
	render(x, y, horizPos, vertPos, commonBuffer);
}
