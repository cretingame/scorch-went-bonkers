#ifndef _TEXTHANDLER_H
#define _TEXTHANDLER_H

/*************************************************/

#include <cstring>
#include <map>

#include "../engine/TextRenderer.h"

using namespace std;

/*************************************************/

struct stringLessThan
{
	bool operator()(const char *s1, const char *s2)
	{
		return strcmp(s1, s2) < 0;
	}
};

/*************************************************/

class TextHandler
{
	private:
		
		map<const char*, TextRenderer*, stringLessThan>		loadedFonts;
		static TextHandler *						theHandler;
	
		TextHandler();
		~TextHandler();
	
	public:
		static TextHandler*	getInstance();
		static void			freeFonts();
	
		TextRenderer *getRenderer(const char *fontName);
};

/*************************************************/

#endif
