#ifndef _STATICIMAGE_H
#define _STATICIMAGE_H

/********************************************************/

#include "../Classes.h"
#include "../menu/StaticText.h"

/********************************************************/

class StaticImage
	: public StaticText
{
	protected:
		
		GLBitmap *image;
	
	public:
		StaticImage(GLBitmap *_image, double _marginTop, double _marginBottom);
		~StaticImage();
	
		void		renderAt(double x, double y);
};

/********************************************************/

#endif
