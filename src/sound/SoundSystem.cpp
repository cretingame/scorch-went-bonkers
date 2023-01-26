#include <allegro.h>
#include <fmod.h>
#include <fmod_errors.h>

#include "../misc/Settings.h"

#include "SoundSystem.h"

/*************************************************************************
                  CLASS "SOUNDSYSTEM" FUNCTIONS
**************************************************************************/

//  STATICS
// *********

FSOUND_SAMPLE *SoundSystem::sounds[SND_MAX];
FMUSIC_MODULE *SoundSystem::modules[MOD_MAX];
bool SoundSystem::loadFlag = false;

//  CONSTRUCTORS AND DESTRUCTORS
// ******************************

SoundSystem::SoundSystem()
{
	setMasterVolume(100);
	setSoundVolume(100);
	setMusicVolume(100);
}

//  STANDARD LOAD-S AND FREE-S
// ****************************

void SoundSystem::loadAll()
{
	static const char *sampleFileNames[SND_MAX] = {"snd/boom.ogg", "snd/shoot.ogg", "snd/shield.ogg",
									"snd/shield_bounce.ogg", "snd/menu_move.ogg", "snd/menu_select.ogg",
									"snd/time_ding.ogg", "snd/time_buzz.ogg"};
	static const char *moduleFileNames[MOD_MAX] = {"mus/menu.mod"};
	
	loadFlag = true;
 	if (!FSOUND_Init(44100, 32, 0))
 	{
 		allegro_message("Couldn't initialize FMOD sound.\n%s", FMOD_ErrorString(FSOUND_GetError()));
 		loadFlag = false;
 	}
 	
	if (loadFlag)
	{
		for (int i = 0; i < SND_MAX; i++)
		{
			sounds[i] = FSOUND_Sample_Load(FSOUND_UNMANAGED, Settings::path(sampleFileNames[i]), FSOUND_LOOP_OFF, 0, 0);
			if (!sounds[i])
				allegro_message("Couldn't load sound file: '%s'", sampleFileNames[i]);
		}
		
		for (int i = 0; i < MOD_MAX; i++)
		{
			modules[i] = FMUSIC_LoadSong(moduleFileNames[i]);
			if (!modules[i])
			{
				int error = FSOUND_GetError();
				if (error != FMOD_ERR_FILE_NOTFOUND)
					allegro_message("Error loading '%s'\n%s", Settings::path(moduleFileNames[i]), FMOD_ErrorString(error));
			}
		}
	}
}

/********************************/

void SoundSystem::freeAll()
{
	for (int i = 0; i < SND_MAX; i++)
	{
		if (sounds[i])
			FSOUND_Sample_Free(sounds[i]);
		sounds[i] = NULL;
	}
	
	for (int i = 0; i < MOD_MAX; i++)
	{
		if (modules[i])
			FMUSIC_FreeSong(modules[i]);
		modules[i] = NULL;
	}
	
	FSOUND_Close();
	
	loadFlag = false;
}

//  VOLUME LEVELS
// ***************

void SoundSystem::setSoundVolume(int percent)
{
	soundVolume = ((float)percent) / 100.0;
}

/********************************/

void SoundSystem::setMusicVolume(int percent)
{
	if (!Settings::setting[SND_MUSIC])
		percent = 0;
	
	musicVolume = ((float)percent) / 100.0;
	
	for (int i = 0; i < MOD_MAX; i++)
		FMUSIC_SetMasterVolume(modules[i], (int)(masterVolume * musicVolume * moduleAtVolume[i] * 256.0));
}

/********************************/

void SoundSystem::setMasterVolume(int percent)
{
	masterVolume = ((float)percent) / 100.0;
	setMusicVolume((int)(musicVolume * 100));
}

//  MUSIC HANDLING
// ****************

void SoundSystem::playMusic(int id, bool looped, float volume)
{
  if ((!modules[id]) || (!loadFlag)) return;

  int realVolume = (int)(masterVolume * musicVolume * volume * 256.0);

  FMUSIC_SetMasterVolume(modules[id], realVolume);
  FMUSIC_SetLooping(modules[id], looped);

  if (!FMUSIC_PlaySong(modules[id]))
    allegro_message("FMOD Error: %s %d", FMOD_ErrorString(FSOUND_GetError()), realVolume);

  moduleAtVolume[id] = volume;
}

//  SOUND HANDLING
// ****************

void SoundSystem::playSound(int id, int pan, float freq, float volume)
{
  if ((!sounds[id]) || (!loadFlag)) return;
  if (!Settings::setting[SND_SFX]) return;

  int realVolume = (int)(masterVolume * soundVolume * volume * 255.0);
  int realFreq = (int)(44100.0 * freq);

  int channel = FSOUND_PlaySoundEx(FSOUND_FREE, sounds[id], NULL, true);

  if (channel == -1)
    allegro_message("FMOD Error: %s", FMOD_ErrorString(FSOUND_GetError()));

  FSOUND_SetPan(channel, pan);
  FSOUND_SetFrequency(channel, realFreq);
  FSOUND_SetVolume(channel, realVolume);
  FSOUND_SetPaused(channel, false);
}

/********************************/

void SoundSystem::cutAllSounds()
{
  if (!loadFlag) return;

  FSOUND_StopSound(FSOUND_ALL);
}

/*******************************************************************************/

SoundSystem *soundSys;



