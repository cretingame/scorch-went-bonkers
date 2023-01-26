#ifndef _MultiOPTION_H
#define _MultiOPTION_H

/***********************************************/

#include <vector>
#include <string>

#include "../Classes.h"
#include "../menu/DescenderOption.h"
#include "../menu/AscenderOption.h"

using namespace std;

/***********************************************/

class MultiOption
	: public DescenderOption
{
	protected:
		vector<string>	values;
		int			*target;
	
	public:
		MultiOption(const char *_text, int *target, const char *_values, double _marginTop = 15.0, double _marginBottom = 15.0, 
			ACTION_CALLBACK _callback = NULL, ACTION_CALLBACK _childCallback = NULL);
	
		void			renderAt(double x, double y);
	
		bool			respondsToLeftRight() {return true;};
		void			action(int keyScancode);
};

/***********************************************/

#endif
