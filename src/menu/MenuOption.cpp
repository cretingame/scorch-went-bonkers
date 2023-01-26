#include <allegro.h>
#include <GL/gl.h>

#include "../misc/TextHandler.h"
#include "MenuOption.h"

/*********************************************/

MenuOption::MenuOption(const char *_text, double _marginTop, double _marginBottom, ACTION_CALLBACK _callback)
	: text(_text), marginTop(_marginTop), marginBottom(_marginBottom),
	  textColor(1.0, 1.0, 1.0), shadowColor(0.0, 0.0, 0.0), textSize(0.4),
	  callback(_callback)
{
	parentMenu = NULL;
	selected = false;
	textRend = TextHandler::getInstance()->getRenderer("wet");
}

/*********************************************/

void MenuOption::select(int fromDirection)
{
	selected = true;
}

/*********************************************/

void MenuOption::deselect(int fromDirection)
{
	selected = false;
}

/*********************************************/

void MenuOption::renderAt(double x, double y)
{
	if (selected)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.3, 0.5, 1.0);
		glBegin(GL_QUADS);
			glVertex2f(x - 240, y - 15); glVertex2f(x + 240, y - 15);
			glVertex2f(x + 240, y + 15); glVertex2f(x - 240, y + 15);
		glEnd();
	}
	
	textRend->setSize(textSize, textSize);
	textRend->render((int)x + 2, (int)y + 2, 0.5, 0.5, shadowColor, text.c_str());
	textRend->render((int)x, (int)y, 0.5, 0.5, textColor, text.c_str());
}

/**********************************************/

void MenuOption::action(int keyScancode)
{
	if (callback)
		callback(this);
}
