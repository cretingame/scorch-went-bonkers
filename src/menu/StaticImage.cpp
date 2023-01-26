#include "../engine/GLBitmap.h"

#include "StaticImage.h"

/***************************************************/

StaticImage::StaticImage(GLBitmap *_image, double _marginBottom, double _marginTop)
	: StaticText("", GLColor(1.0, 1.0, 1.0), _marginBottom, _marginTop), image(_image)
{
	image->setOrigin(0.5, 0.5);
}

/***************************************************/

StaticImage::~StaticImage()
{
	delete image;
}

/***************************************************/

void StaticImage::renderAt(double x, double y)
{
	image->draw(x, y);
}
