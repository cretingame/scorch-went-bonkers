#include "../misc/TextHandler.h"
#include "../engine/TextRenderer.h"

#include "TargetBoard.h"

/********************************************/

TargetBoard::TargetBoard()
	: Entity(790.0, 7.0)
{
	shotsHit = 0;
	shotsFired = 0;
}

/********************************************/

void TargetBoard::render()
{
	glDisable(GL_ALPHA_TEST);
	
	double accuracy =  shotsFired ? (double)shotsHit * 100 / shotsFired : 0.0;
	
	TextRenderer *tr = TextHandler::getInstance()->getRenderer("wet");
	
	tr->setSize(0.4, 0.4);
	tr->printf(x + 1, y + 1, 1.0, 0.0, GLColor(0.0, 0.0, 0.4, 0.6), "Hits: %d", shotsHit);
	tr->printf(x, y, 1.0, 0.0, GLColor(1.0, 1.0, 1.0, 0.6), "Hits: %d", shotsHit);
	tr->printf(x + 1, y + 19, 1.0, 0.0, GLColor(0.0, 0.0, 0.4, 0.6), "Shots fired: %d", shotsFired);
	tr->printf(x, y + 18, 1.0, 0.0, GLColor(1.0, 1.0, 1.0, 0.6), "Shots fired: %d", shotsFired);
	tr->printf(x + 1, y + 37, 1.0, 0.0, GLColor(0.0, 0.0, 0.4, 0.6), "Accuracy: %.1lf%%", accuracy);
	tr->printf(x, y + 36, 1.0, 0.0, GLColor(1.0, 1.0, 1.0, 0.6), "Accuracy: %.1lf%%", accuracy);

}
