#ifndef _KEYREADER_H
#define _KEYREADER_H

/******************************************/

#include <list>

using namespace std;

typedef list<int>::iterator KEY_ITERATOR;

/******************************************/

class KeyReader{
	
	public:

		static list<int>		scancodes;
	
		static void			clearBuffer();
		static void			update();
};

/******************************************/

#endif
