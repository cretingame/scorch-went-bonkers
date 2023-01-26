#include "AscenderOption.h"

/********************************************/

void AscenderOption::action(int keyScancode)
{
	parentMenu->getParentMenu()->restoreFocus();
	MenuOption::action(keyScancode);
};

