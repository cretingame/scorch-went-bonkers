#include <GL/gl.h>
#include <cstring>

#include "../menu/Menu.h"
#include "../menu/ChoiceOption.h"
#include "../menu/StaticText.h"
#include "../engine/TextRenderer.h"
#include "MultiOption.h"

/*************************************************/

MultiOption::MultiOption(const char *_text, int *_target, const char *valueString, double _marginTop, double _marginBottom, 
	ACTION_CALLBACK _callback, ACTION_CALLBACK childCallback)
	: DescenderOption((string(_text) + ":").c_str(), NULL, _marginTop, _marginBottom, _callback), target(_target)
{
	char *temp, *tok;
	
	childMenu = new Menu(400, 300);
	childMenu->addOption(new StaticText(_text, GLColor(1.0, 1.0, 0.0), 0.0, 35.0, 0.6),
		Effect(0.0, 30.0, E_TOP), Effect(0.0, 30.0, E_ENLARGE));
	
	temp = new char[strlen(valueString) + 1];
	strcpy(temp, valueString);
	tok = strtok(temp, ",");

	while(tok)
	{
		childMenu->addOption(new ChoiceOption(tok, target, values.size(), 15.0, 15.0, childCallback));
		values.push_back(tok);
		
		tok = strtok(NULL, ",");
	}
	
	delete [] temp;
}

/*************************************************/

void MultiOption::renderAt(double x, double y)
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
	textRend->render((int)x - 233, (int)y + 2, 0.0, 0.5, shadowColor, text.c_str());
	textRend->render((int)x - 235, (int)y, 0.0, 0.5, textColor, text.c_str());

	textRend->render((int)x + 235, (int)y + 2, 1.0, 0.5, shadowColor, values[*target].c_str());
	textRend->render((int)x + 233, (int)y, 1.0, 0.5, textColor, values[*target].c_str());
}

/*************************************************/

void MultiOption::action(int keyScancode)
{
	switch(keyScancode)
	{
		case KEY_LEFT:
			(*target)--;
			if (*target < 0)
				(*target) = values.size() - 1;
			MenuOption::action(keyScancode);
			break;
			
		case KEY_RIGHT:
			(*target)++;
			if (*target >= (int)values.size())
				(*target) = 0;
			MenuOption::action(keyScancode);
			break;

		default:
			childMenu->setSelection(*target + 1);
			DescenderOption::action(keyScancode);
			break;
	}
}
