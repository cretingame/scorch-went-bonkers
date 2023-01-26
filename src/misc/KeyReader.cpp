#include <allegro.h>
#include "KeyReader.h"


list<int>	KeyReader::scancodes;

void KeyReader::clearBuffer()
{
	while(keypressed())
		readkey();
}

void KeyReader::update()
{
	scancodes.clear();
	
	while(keypressed())
	{
		int scancode = readkey() >> 8;
		scancodes.push_back(scancode);
	}
}
