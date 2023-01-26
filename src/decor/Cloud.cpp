#include <GL/gl.h>

#include "../misc/Random.h"
#include "../misc/Theme.h"
#include "../engine/GLBitmap.h"
#include "Cloud.h"

/*************************************************/

Cloud::Cloud(Theme *_theme, int _typeNr, bool offRight)
	: Entity(0.0, 0.0), theme(_theme), typeNr(_typeNr)
{
	aliveFlag = true;
	
	scale = randomFloat(0.8, 1.0);
	width	= theme->getCloudImage(typeNr)->width() * scale;
	height	= theme->getCloudImage(typeNr)->height() * scale;
	
	if (offRight)
		setX(801.0);
	else
		setX(randomFloat(-width, 800.0));
	
	double minTop = theme->getCloudsTop();
	double maxBottom = theme->getCloudsBottom();
	setY(randomFloat(minTop, maxBottom - height));
	
	vx = randomFloat(-0.02, -0.05);
}

/*************************************************/

void Cloud::step(double dt)
{
	x += vx * dt;
	
	if (x < -width)
		aliveFlag = false;
}

/*************************************************/

void Cloud::render()
{
	GLBitmap *bmp = theme->getCloudImage(typeNr);
	bmp->setScale(scale, scale);
	bmp->draw(x, y);	
}
