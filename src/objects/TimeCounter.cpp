#include <cmath>

#include "../misc/TextHandler.h"
#include "../engine/TextRenderer.h"
#include "../sound/SoundSystem.h"

#include "TimeCounter.h"

/************************************************/

TimeCounter::TimeCounter(double _timeLimit, double _timeRescale)
	: Entity(24.0, 20.0), timeLimit(_timeLimit), timeRescale(_timeRescale)
{
	basicSize = 0.6;
	buff = 1.0;
	time = 0.0;
	animProgress = 0.0;
	lastTimeDisplayed = 0;
}

/************************************************/

void TimeCounter::step(double dt)
{
	time += dt / timeRescale;
	
	if (buff > 1.00)
		buff -= 0.04 * dt;
	
	if (timeLimit - time < 550.0)
	{
		animProgress = (time - timeLimit + 550.0) / 50.0;
		if (animProgress > 1.0) animProgress = 1.0;
	}
	else if (animProgress > 0.0)
	{
		animProgress -= dt * 0.02;
		if (animProgress < 0.0) animProgress = 0.0;
	}

	x = 24.0 * (1.0 - animProgress) + 400.0 * animProgress;
	y = 20.0 * (1.0 - animProgress) + 100.0 * animProgress;
	basicSize = 0.6 + animProgress * 0.7;
}

/************************************************/

void TimeCounter::render()
{
	glDisable(GL_ALPHA_TEST);
	
	int timeDisplayed = (int)(std::ceil((timeLimit - time) / 100.0));
	
	if (timeDisplayed > 600) return;
	if (timeDisplayed < 0) timeDisplayed = 0;
	
	if (timeDisplayed < lastTimeDisplayed)
	{
		if (timeDisplayed <= 5 && timeDisplayed > 0)
			soundSys->playSound(SND_TIME_DING, 128, 0.5, 1.0);
		buff = 1.3;
	}
	
	TextRenderer *textRend = TextHandler::getInstance()->getRenderer("wet");
	
	textRend->setSize(basicSize * buff, basicSize * buff);
	textRend->printf((int)x+2, (int)y+2, 0.5, 0.5, GLColor(0.0, 0.0, 0.4, 0.4), "%d", timeDisplayed);
	textRend->printf((int)x, (int)y, 0.5, 0.5, GLColor(1.0, 1.0, 1.0, 0.4), "%d", timeDisplayed);
	
	lastTimeDisplayed = timeDisplayed;
}
