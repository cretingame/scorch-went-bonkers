#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <alogg.h>
 
#include "../misc/Settings.h"

#include "SoundSystem.h"

/*************************************************************************
                  CLASS "SOUNDSYSTEM" FUNCTIONS
**************************************************************************/

//  STATICS
// *********

SAMPLE *SoundSystem::sounds[SND_MAX];
DUH *SoundSystem::modules[MOD_MAX];
bool SoundSystem::loadFlag = false;

AL_DUH_PLAYER *SoundSystem::duh_player = NULL;
pthread_t SoundSystem::duh_player_thread;
pthread_mutex_t SoundSystem::duh_player_mutex;

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
	reserve_voices(32, 0);
	if(install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL))
 	{
		if (Settings::setting[GFX_FULLSCREEN]) /* don't use allegro_message when fullscreen! */
			fprintf(stderr, "Couldn't initialize sound.\n%s\n", allegro_error);
		else
 			allegro_message("Couldn't initialize sound.\n%s", allegro_error);
 		loadFlag = false;
 	}
 	
	if (loadFlag)
	{
		dumb_register_packfiles();
		pthread_mutex_init(&duh_player_mutex, NULL);
		for (int i = 0; i < SND_MAX; i++)
		{
			ALOGG_OGG *ogg;
			FILE *f = fopen(Settings::path(sampleFileNames[i]), "r");
			sounds[i] = NULL;
			if (!f)
			{
				allegro_message("Couldn't load sound file: '%s': %s", sampleFileNames[i], strerror(errno));
				continue;
			}
			ogg = alogg_create_ogg_from_file(f);
			if (!ogg)
			{
				fclose(f);
				allegro_message("Couldn't load sound file: '%s'", sampleFileNames[i]);
				continue;
			}
			sounds[i] = alogg_create_sample_from_ogg(ogg);
			alogg_destroy_ogg(ogg);
			if (!sounds[i])
				allegro_message("Couldn't load sound file: '%s'", sampleFileNames[i]);
		}
		
		for (int i = 0; i < MOD_MAX; i++)
		{
			modules[i] = dumb_load_mod_quick(Settings::path(moduleFileNames[i]));
			if (!modules[i])
			{
				allegro_message("Error loading '%s'\n", Settings::path(moduleFileNames[i]));
			}
		}
	}
}

/********************************/

void SoundSystem::freeAll()
{
	if (!loadFlag) return;
	
	for (int i = 0; i < SND_MAX; i++)
	{
		if (sounds[i])
			destroy_sample(sounds[i]);
		sounds[i] = NULL;
	}
	
	for (int i = 0; i < MOD_MAX; i++)
	{
		if (modules[i])
			unload_duh(modules[i]);
		modules[i] = NULL;
	}
	
	dumb_exit();
	remove_sound();
	
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
	if (SoundSystem::duh_player)
	{
	    pthread_mutex_lock(&SoundSystem::duh_player_mutex);
	    al_duh_set_volume(SoundSystem::duh_player, musicVolume);
	    pthread_mutex_unlock(&SoundSystem::duh_player_mutex);
	}
}

/********************************/

void SoundSystem::setMasterVolume(int percent)
{
	masterVolume = ((float)percent) / 100.0;
	set_volume((int)(masterVolume * 255.0), 0);
}

//  MUSIC HANDLING
// ****************

static void *duh_player_thread_function(void *arg)
{
    while(1)
    {
	pthread_mutex_lock(&SoundSystem::duh_player_mutex);
	/* this may be nuked if things fail in set_music_pattern */
	if (SoundSystem::duh_player)
	    al_poll_duh(SoundSystem::duh_player);
	pthread_mutex_unlock(&SoundSystem::duh_player_mutex);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	usleep(10000);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    }
    return NULL;
}

void SoundSystem::playMusic(int id, bool looped, float volume)
{
  stopMusic();
  
  if ((!modules[id]) || (!loadFlag)) return;

  duh_player = al_start_duh(modules[id], 2, 0, musicVolume, 4096, 44100);
  if (!looped)
    dumb_it_set_loop_callback(
      duh_get_it_sigrenderer(al_duh_get_sigrenderer(duh_player)),
      dumb_it_callback_terminate, NULL);

  if (pthread_create(&duh_player_thread, NULL,
          duh_player_thread_function, NULL))
  {
    al_stop_duh(duh_player);
    duh_player = NULL;
  }
}

void SoundSystem::stopMusic()
{
    if (duh_player)
    {
        /* stop the player thread */
        pthread_cancel(duh_player_thread);
        pthread_join(duh_player_thread, NULL);
        /* and clean up */
        al_stop_duh(duh_player);
        duh_player = NULL;
    }
}

//  SOUND HANDLING
// ****************

void SoundSystem::playSound(int id, int pan, float freq, float volume)
{
  if ((!sounds[id]) || (!loadFlag)) return;
  if (!Settings::setting[SND_SFX]) return;
  
  play_sample(sounds[id], (int)(soundVolume * volume * 255.0), pan,
    (int)(freq * 1000.0), 0);
}

/********************************/

void SoundSystem::cutAllSounds()
{
  if (!loadFlag) return;
  
  for (int i = 0; i < SND_MAX; i++)
  {
    if (sounds[i])
      stop_sample(sounds[i]);
  }
}

/*******************************************************************************/

SoundSystem *soundSys;
