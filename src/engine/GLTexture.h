#ifndef _GLTEXTURE_H
#define _GLTEXTURE_H

#include <allegro.h>
#include <GL/gl.h>

/********************************************/

class GLTexture
{
	private:
		
		public:
		GLuint texture;
	
		GLTexture(GLuint _texture) : texture(_texture) {};
	
	public:
		
		~GLTexture() {glDeleteTextures(1, &texture);};

		static GLTexture *create(BITMAP *bmp, GLuint format, int aglFlags);
		static GLTexture *load(const char *fileName, GLuint format, int aglFlags);
	
		void bind();
	
};

/********************************************/

#endif
