#include <alleggl.h>

#include "../misc/Settings.h"
#include "GLBitmap.h"

GLBitmap::GLBitmap()
{
	texture = 0;
}

GLBitmap::~GLBitmap()
{
	if (texture)
		glDeleteTextures(1, &texture);
}

GLBitmap* GLBitmap::create(BITMAP *bmp, int flags)
{
	GLBitmap *glb = new GLBitmap();
	
	// parses the flags

	int flagNoAlpha   = (flags & TF_NO_ALPHA);
	int flagMagicPink = (flags & TF_MAGIC_PINK);

	// determine parameters for AGL
	
	glb->hasAlpha = !flagNoAlpha;
	int aglInternalFormat, aglFlags;

	aglFlags =		((!flagNoAlpha) && (!flagMagicPink))  ? AGL_TEXTURE_HAS_ALPHA : 0;
	aglFlags |=		(flagMagicPink) ? AGL_TEXTURE_MASKED : 0;
	
	aglInternalFormat = (flagNoAlpha) ? GL_RGB8 : GL_RGBA8;

	// sets original widths and heights

	glb->originalWidth = bmp->w; glb->originalHeight = bmp->h;
	
	int newWidth = bmp->w, newHeight = bmp->h;
	bool needTemp = false;
	BITMAP *temp = NULL;

	// determines if temporary bitmap is needed, and if it is, determines its size
	if (bmp->w & (bmp->w - 1))
	{
		needTemp = true;
		newWidth = 1;
		while(newWidth < bmp->w) newWidth <<= 1;
	}
	if (bmp->h & (bmp->h - 1))
	{
		needTemp = true;
		newHeight = 1;
		while(newHeight < bmp->h) newHeight <<= 1;
	}
	
	//~ if (newHeight > newWidth)
		//~ newWidth = newHeight;
	//~ if (newWidth > newHeight)
		//~ newHeight = newWidth;
	
	// actual texture creation part
	if (needTemp)
	{
		temp = create_bitmap_ex(bitmap_color_depth(bmp), newWidth, newHeight);
		
		if (!temp)
		{
			delete glb;
			return NULL;
		}
		blit(bmp, temp, 0, 0, 0, 0, bmp->w, bmp->h);
		if (!flagNoAlpha)
		{
			drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
			set_write_alpha_blender();
			line(temp, bmp->w, 0, bmp->w, bmp->h + 1, 0);
			line(temp, 0, bmp->h, bmp->w + 1, bmp->h, 0);
			drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		}
		glb->texture = allegro_gl_make_texture_ex(aglFlags, temp, aglInternalFormat);
		destroy_bitmap(temp);
	}
	else
		glb->texture = allegro_gl_make_texture_ex(aglFlags, bmp, aglInternalFormat);
	
	// check if the texture was actually created
	if (glb->texture == 0)
	{
		delete glb;
		return NULL;
	}
	
	// bind the texture, check what we actually got
	int actualWidth, actualHeight;
	glBindTexture(GL_TEXTURE_2D, glb->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &actualWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &actualHeight);
	
	// calculate corner texture coordinates
	glb->textureRight =  (float)glb->originalWidth / actualWidth;
	glb->textureBottom = (float)glb->originalHeight / actualHeight;

	// set the proper transparency mode
	if (flagNoAlpha)
		glb->transpMode = TM_NONE;
	else if (flagMagicPink)
		glb->transpMode = TM_ALPHA_CUT;
	else
		glb->transpMode = TM_ALPHA_BLEND;

	// set default parameters
	glb->rotation = 0.0f;
	glb->scaleX = glb->scaleY = 1.0f;
	
	glb->setOrigin(0.0, 0.0);

	glb->doRotate = glb->doScale = glb->doTint = false;

	return glb;
}

GLBitmap* GLBitmap::load(const char *fileName, int flags, bool inform)
{
	BITMAP *bmp = load_bitmap(Settings::path(fileName), NULL);
	
	if (!bmp)
	{
		if (inform)
			allegro_message("Unable to load bitmap '%s'", fileName);
		return NULL;
	}
	
	GLBitmap *ret = GLBitmap::create(bmp, flags);
	
	destroy_bitmap(bmp);
	
	return ret;
}

void GLBitmap::setOrigin(GLfloat x, GLfloat y)
{
	 originX = x; originY = y;
	 offsetX = -originX * originalWidth;
	 offsetY = -originY * originalHeight;
	 revOffsetX = (1.0 - originX) * originalWidth;
	 revOffsetY = (1.0 - originY) * originalHeight;
}

void GLBitmap::draw(GLfloat x, GLfloat y)
{
	// is texture valid?
	if (!texture)
		return;
	
	// store state
	glPushMatrix();
//	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT); // ** don't ask why GL_TEXTURE_BIT is here...
	
	// enable what has to be enabled
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// translate to the proper position
	glTranslatef(x, y, 0.0);
	
	// rotation
	if (doRotate)
		glRotatef(rotation, 0.0f, 0.0f, -1.0f);

	// scaling
	if (doScale)
		glScalef(scaleX, scaleY, 1.0f);

	// transparency
	switch(transpMode)
	{
		case TM_NONE:
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			break;
		case TM_ALPHA_CUT:
			glDisable(GL_BLEND);
			glAlphaFunc(GL_GREATER, 0.5f);
			glEnable(GL_ALPHA_TEST);
			break;
		case TM_ALPHA_BLEND:
			glDisable(GL_ALPHA_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			break;
		case TM_ALPHA_ADDITIVE:
			glDisable(GL_ALPHA_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_BLEND);
			break;
		default:
			return; // shouldn't be here, the texture is somehow in invalid state.
	}

	// tinting
	if (doTint)
	{
		glColor4f(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
	}
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);

	// drawing
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(offsetX, offsetY);
		glTexCoord2f(textureRight, 0.0);
		glVertex2f(revOffsetX, offsetY);
		glTexCoord2f(textureRight, textureBottom);
		glVertex2f(revOffsetX, revOffsetY);
		glTexCoord2f(0.0, textureBottom);
		glVertex2f(offsetX, revOffsetY);
	glEnd();
	
	// restore state
//	glPopAttrib();
	glPopMatrix();
}
