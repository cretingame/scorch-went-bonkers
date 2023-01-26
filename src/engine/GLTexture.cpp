#include <GL/gl.h>
#include <allegro.h>
#include <alleggl.h>

#include "../misc/Settings.h"
#include "GLTexture.h"

/*******************************************/

GLTexture* GLTexture::create(BITMAP *bmp, GLuint format, int aglFlags)
{
	int rawTexture = allegro_gl_make_texture_ex(aglFlags, bmp, format);
	
	if (rawTexture == 0)
		return NULL;
	
	GLTexture *tex = new GLTexture(rawTexture);
	
	return tex;
}

/*******************************************/

GLTexture* GLTexture::load(const char *fileName, GLuint format, int aglFlags)
{
	BITMAP *bmp = load_bitmap(Settings::path(fileName), NULL);
	
	if (!bmp)
	{
		allegro_message("Unable to load texture '%s'.", fileName);
		return NULL;
	}
	
	GLTexture *ret = GLTexture::create(bmp, format, aglFlags);
	
	destroy_bitmap(bmp);
	
	return ret;
}

/*******************************************/

void GLTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

