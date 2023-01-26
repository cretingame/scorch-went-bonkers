#include "../menu/Menu.h"
#include "StaticText.h"

void StaticText::select(int fromDir)
{
	if (fromDir == MDIR_DOWN)
		parentMenu->selectAbove();
	else
		parentMenu->selectBelow();
}

void StaticText::deselect(int toDir) 
{}
