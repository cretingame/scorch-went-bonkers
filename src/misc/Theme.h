#ifndef _THEME_H
#define _THEME_H

/*******************************************/

#include "../Classes.h"
#include "../engine/GLColor.h"

/*******************************************/

class Theme
{
	protected:
		
		int cloudsTypeCount;
		int cloudsTop, cloudsBottom;
		int maxClouds;
	
		GLColor backgroundColor;
		GLBitmap *backgroundImage;
		GLBitmap **cloudImages;
	
		Theme();
	
	public:
		
		static Theme*		load(const char *name);
		~Theme();
	
		int getCloudsTypeCount()	{return cloudsTypeCount;};
		int getCloudsTop()		{return cloudsTop;};
		int getCloudsBottom()		{return cloudsBottom;};
		int getMaxClouds()		{return maxClouds;};
		
		GLColor getBackColor()			{return backgroundColor;};
		GLBitmap *getBackImage()		{return backgroundImage;};
		GLBitmap *getCloudImage(int nr)		{return cloudImages[nr];};
};

/*******************************************/

#endif
