#include <cstdio>

#include <allegro.h>
#include <alleggl.h>

#include "../engine/GLTexture.h"
#include "../engine/GLBitmap.h"
#include "Theme.h"

/*********************************************/

Theme *Theme::load(const char *name)
{
	static char buf[200];
	
	set_config_file("misc/themes.ini");
	
	if (!get_config_int(name, "exists", 0))
	{
		allegro_message("Theme '%s' doesn't exist.", name);
		return NULL;
	}
	
	Theme *ret = new Theme();
	ret->cloudsTypeCount = get_config_int(name, "clouds_count", 0);
	ret->cloudsTop = get_config_int(name, "clouds_layer_top", 0);
	ret->cloudsBottom = get_config_int(name, "clouds_layer_bottom", 0);
	ret->maxClouds = get_config_int(name, "clouds_on_screen", 0);
	
	int r, g, b;
	r = get_config_int(name, "background_color_r", 0);
	g = get_config_int(name, "background_color_g", 0);
	b = get_config_int(name, "background_color_b", 0);
	ret->backgroundColor = GLColor(r, g, b);
	
	sprintf(buf, "gfx/back/%s.jpg", name);
	ret->backgroundImage = GLBitmap::load(buf, GLBitmap::TF_NO_ALPHA, false);
	if (!ret->backgroundImage)
	{
		sprintf(buf, "gfx/back/%s.tga", name);
		ret->backgroundImage = GLBitmap::load(buf, GLBitmap::TF_NO_ALPHA);
		if (!ret->backgroundImage)
		{
			delete ret;
			return NULL;
		}
	}
	
	if (ret->cloudsTypeCount > 0)
	{
		ret->cloudImages = new GLBitmap*[ret->cloudsTypeCount];
		for (int i = 0; i < ret->cloudsTypeCount; i++)
			ret->cloudImages[i] = NULL;
		
		for (int i = 0; i < ret->cloudsTypeCount; i++)
		{
			sprintf(buf,"gfx/clouds/%s_%d.tga", name, i + 1);
			BITMAP *bmp = load_bitmap(buf, NULL);
			if (!bmp)
			{
				allegro_message("Unable to load bitmap '%s'", buf);
				delete ret; return NULL;
			}
			ret->cloudImages[i] = GLBitmap::create(bmp);
			if (!ret->cloudImages[i])
			{
				allegro_message("Unable to create cloud bitmap number %d. %d", i, bitmap_color_depth(bmp));
				destroy_bitmap(bmp);
				delete ret; return NULL;
			}
			
			destroy_bitmap(bmp);
		}
	}
	
	return ret;
}

/*********************************************/

Theme::Theme()
{
	backgroundImage = NULL;
	cloudImages = NULL;
}

/*********************************************/

Theme::~Theme()
{
	if (backgroundImage)
		delete backgroundImage;
	if (cloudImages)
	{
		for (int i = 0; i < cloudsTypeCount; i++)
			if (cloudImages[i])
				delete cloudImages[i];
		delete[] cloudImages;
	}	
}
