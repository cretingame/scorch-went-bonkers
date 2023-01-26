#include <allegro.h>
#include "Quitting.h"

/******************************************/

bool Quitting::quitFlag = false;

/******************************************/

void close_button_handler(void)
{
	Quitting::triggerQuit();
}
END_OF_FUNCTION(close_button_handler)

/******************************************/

void Quitting::triggerQuit() {
	quitFlag = true;
}

/******************************************/

void Quitting::registerHandler() {
	set_close_button_callback(close_button_handler);
}

/******************************************/

bool Quitting::applicationQuitting() {
	return quitFlag;
}
