#ifndef _SETTINGS_H
#define _SETTINGS_H

/************************************************/

enum SettingTypes
{
	GFX_FULLSCREEN,
	GFX_EXPLOSION,
	GFX_DEBRIS,
	GFX_SMOKE,
	GFX_CLOUDS,
	
	GPL_GRAVITY,
	GPL_AIR_FRICTION,
	GPL_GAME_SPEED,
	
	GPL_SHIELD_TIME,
	GPL_DOWN_TIME,
	GPL_BLAST_RADIUS,
	
	SND_MUSIC,
	SND_SFX,
	
	ACC_CYCLE_SPEED,
	ACC_TAP_SPEED,
	ACC_SLOW_SHOOTING,
	
	LAST_SETTING
};

/************************************************/

extern const char *settingSections[];
extern const char *settingNames[];
extern const int settingDefaults[];

/************************************************/

class Settings
{
	public:
	
	static int		setting[LAST_SETTING];
	static char		basePath[512], filenameBuffer[512];
	
	static void		loadSettings();
	static void		saveSettings();
	static char*	path(const char *filenamePart);
};

/************************************************/

#endif
