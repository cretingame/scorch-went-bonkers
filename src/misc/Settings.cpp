#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

#include "Settings.h"

/**************************************************/

int Settings::setting[LAST_SETTING];
char Settings::basePath[512];
char Settings::filenameBuffer[512];

/**************************************************/

const char *settingSections[LAST_SETTING] =
{
	"Graphics", "Graphics", "Graphics", 
	"Graphics", "Graphics",
	
	"Gameplay", "Gameplay", "Gameplay",
	"Gameplay", "Gameplay", "Gameplay",
	
	"Sound", "Sound",
	
	"Accesibility", "Accesibility", "Accessibility"
};

const char *settingNames[LAST_SETTING] =
{
	"fullscreen", "explosion_detail", "debris_detail",
	"smoke_detail", "clouds",
	
	"gravity", "friction", "speed",
	"shield_time", "down_time", "blast_radius",
	
	"music", "sfx",
	
	"cycle_speed", "tap_speed", "slow_shoot"
};

const int settingDefaults[LAST_SETTING] =
{
	0, // windowed
	3, // full detail explosions
	3, // loads o' debris
	3, // good-looking smoke
	1, // clouds enabled
	
	2, // medium gravity
	2, // medium air fric
	2, // medium game speed
	
	2, // medium shield time
	2, // medium down time
	1, // medium blast radius
	
	1, // music on
	1, // sound on
	
	2, // normal cycle speed
	0, // normal tap speed
	0  // slow shooting off
};

/**************************************************/

void Settings::loadSettings()
{
#ifdef __unix__
	char *home = getenv("HOME");
	snprintf(filenameBuffer, sizeof(filenameBuffer), "%s/.swb.ini", home? home:".");
	override_config_file(filenameBuffer);
#else
	set_config_file("settings.ini");
#endif

	for (int i = 0; i < LAST_SETTING; i++)
		setting[i] = get_config_int(settingSections[i], settingNames[i], settingDefaults[i]);

#ifdef __unix__
	snprintf(basePath, sizeof(basePath), DATADIR);
#else
	char tempPath[512];
	get_executable_name(tempPath, 512);
	replace_filename(basePath, tempPath, "", 512);
#endif
}

void Settings::saveSettings()
{
#ifdef __unix__
	char *home = getenv("HOME");
	snprintf(filenameBuffer, sizeof(filenameBuffer), "%s/.swb.ini", home? home:".");
	override_config_file(filenameBuffer);
#else
	set_config_file("settings.ini");
#endif
	
	for (int i = 0; i < LAST_SETTING; i++)
		set_config_int(settingSections[i], settingNames[i], setting[i]);
	
	flush_config_file();
}

char* Settings::path(const char *filenamePart)
{
	make_absolute_filename(filenameBuffer, basePath, filenamePart, 512);
	return filenameBuffer;
}
