#include "../engine/TextRenderer.h"
#include "../misc/TextHandler.h"

#include "TextFloat.h"

/*******************************************/

TextFloat::TextFloat(double _x, double _y, const char *_text, GLColor _color, double _size)
	: Entity(_x, _y), text(_text), color(_color), size(_size)
{
	opacity = 1.0;
}

/*******************************************/

void TextFloat::step(double dt)
{
	y 	  -= 0.15 * dt;
	opacity -= 0.005 * dt;
	
	if (opacity < 0.0)
		opacity = 0.0;
	
	color.a = opacity;
}

/********************************************/

void TextFloat::render()
{
	TextRenderer *textRend = TextHandler::getInstance()->getRenderer("wet");
	
	textRend->setSize(size, size);
	textRend->render(x + 1, y + 1, 0.5, 0.5, GLColor(0.0, 0.0, 0.0, opacity), text.c_str());
	textRend->render(x, y, 0.5, 0.5, color, text.c_str());
}
