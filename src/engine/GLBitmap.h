#ifndef _GLBITMAP_H
#define _GLBITMAP_H

/**********************************************************/

#include <allegro.h>
#include <GL/gl.h>

/**********************************************************/

#include "GLColor.h"

/**********************************************************/

class GLBitmap
{
	public:
		
		enum	TransparencyMode {TM_NONE = 0, TM_ALPHA_CUT = 1, TM_ALPHA_BLEND = 2, TM_ALPHA_ADDITIVE = 3};
		enum	TransformFlags   {TF_MAGIC_PINK = 1, TF_NO_ALPHA = 2};
	
	private:
	
		GLuint			originalWidth, originalHeight;
		GLuint			texture;
		GLfloat			textureRight, textureBottom;
		bool			hasAlpha;
	
		GLfloat		originX, originY;
		GLfloat		offsetX, offsetY, revOffsetX, revOffsetY;
		
		GLfloat		scaleX, scaleY;
		GLfloat		rotation;
		GLColor		tintColor;
	
		TransparencyMode	transpMode;
	
		bool		doScale, doRotate, doTint;
	
	private:
		
	GLBitmap();
	
	public:
	
	~GLBitmap();
	
	static GLBitmap*	create(BITMAP *bmp, int transformFlags = 0);
	static GLBitmap*	load(const char *fileName, int transformFlags = 0, bool inform = true);

	void		setRotation(GLfloat rot) {rotation = rot; doRotate = true;};
	void		setScale(GLfloat x, GLfloat y) {scaleX = x; scaleY = y; doScale = true;};
	void		setTint(GLColor color) {tintColor = color; doTint = true;};
	void		setOrigin(GLfloat x, GLfloat y);
	void		setTransparencyMode(TransparencyMode mode) {transpMode = mode;};
	
	void		allowRotation(bool allow) {doRotate = allow;};
	void		allowScale(bool allow) {doScale = allow;};
	void		allowTint(bool allow) {doTint = allow;};
	
	void		draw(GLfloat x, GLfloat y);
	
	double		width()  {return originalWidth;};
	double		height() {return originalHeight;};
};

/**********************************************************/

#endif
