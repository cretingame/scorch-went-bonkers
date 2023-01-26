#include <map>
#include <string>
#include <cstring>

#include <allegro.h>

#include "../misc/Settings.h"
#include "TextHandler.h"

using namespace std;

/*****************************************/

TextHandler *TextHandler::theHandler = NULL;

/*****************************************/

TextHandler::TextHandler()
{
}

TextHandler::~TextHandler()
{
	map<const char*, TextRenderer*, stringLessThan>::iterator it;
	
	for (it = loadedFonts.begin(); it != loadedFonts.end(); it++)
	{
		if ((*it).second)
			delete (*it).second;
	}
}
/*****************************************/

TextHandler *TextHandler::getInstance()
{
	if (!theHandler)
		theHandler = new TextHandler();
	
	return theHandler;
}

TextRenderer *TextHandler::getRenderer(const char *fontName)
{
	if (!loadedFonts[fontName])
	{
		char picPath[512], metricsPath[512];
		strcpy(picPath,     Settings::path(("fonts/" + string(fontName) + ".pcx").c_str()));
		strcpy(metricsPath, Settings::path(("fonts/" + string(fontName) + ".metrics").c_str()));
		TextRenderer *tr = TextRenderer::load(picPath, metricsPath); 
		
		if (!tr)
		{
			allegro_message("Unable to load font '%s'.", fontName);
		}
		loadedFonts[fontName] = tr;
	}
	
	return loadedFonts[fontName];
}

/*****************************************/

void  TextHandler::freeFonts()
{
	if (theHandler)
		delete theHandler;
	
	theHandler = NULL;
}
