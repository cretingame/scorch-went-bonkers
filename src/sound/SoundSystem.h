#ifndef _SOUNDSYSTEM_H
#define _SOUNDSYSTEM_H

/****************************************************************************/

#include <fmod.h>

/****************************************************************************/

enum SoundKinds {SND_BOOM, SND_SHOOT, SND_SHIELD, SND_BOUNCE, SND_MENU_MOVE, SND_MENU_SELECT, SND_TIME_DING,
			SND_TIME_BUZZ, SND_MAX};

enum MusicKinds {MOD_MENU, MOD_MAX};

/****************************************************************************/

class SoundSystem
{
  protected:

  	// a flag to check if initialization went all right

    static bool loadFlag;
    
    // an array of all samples

    static FSOUND_SAMPLE *sounds[SND_MAX];
    static FMUSIC_MODULE *modules[MOD_MAX];

    // volumes

    float soundVolume, musicVolume, masterVolume;
    float moduleAtVolume[MOD_MAX];

  public:

    // standard load-s and free-s

    static void loadAll();
    static void freeAll();

    // volume levels

    void setSoundVolume(int percent);
    void setMusicVolume(int percent);
    void setMasterVolume(int percent);

    // sound handling

    void playSound(int id, int pan = 128, float freq = 1.0, float volume = 1.0);
    void playMusic(int id, bool looped = true, float volume = 1.0);
    void cutAllSounds();

    // constructors and destructors

    SoundSystem();
};

/****************************************************************************/

extern SoundSystem *soundSys;

/****************************************************************************/

#endif

