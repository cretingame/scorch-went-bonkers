#include <allegro.h>

#if defined (ALLEGRO_WINDOWS)

#include <winalleg.h>
#include <windows.h>

#elif defined (ALLEGRO_UNIX) || defined (ALLEGRO_MACOSX) || defined (ALLEGRO_LINUX)
#include <sys/time.h>

#endif


/** SYSTEM SPECIFIC TIME ROUTINES **/

#if defined (ALLEGRO_WINDOWS)

// QueryPerformanceCounter system

bool inited = 0;

LONG_LONG getTimerValue()
{
	if (!inited)
		timeBeginPeriod(2);
	
	LONG_LONG timer;
	QueryPerformanceCounter((LARGE_INTEGER*)&timer);
	return timer;
}

LONG_LONG getTimerFrequency()
{
	LONG_LONG frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	return frequency;
}

#elif defined (ALLEGRO_UNIX) || defined (ALLEGRO_LINUX) || defined (ALLEGRO_MACOSX)

// GTOD system

LONG_LONG getTimerValue()
{
	LONG_LONG timer;
	
	timeval t;
	gettimeofday(&t, NULL);
	
	timer = (LONG_LONG)t.tv_sec * 1000000 + t.tv_usec;
	return timer;
}

LONG_LONG getTimerFrequency()
{
	return 1000000;
}

#else

// Legacy system

volatile LONG_LONG timer;
bool callbackInstalled = false;

void timerCallback()
{
	timer++;
}

LONG_LONG getTimerValue()
{
	if (!callbackInstalled)
	{
		timer = 0;
		install_int_ex(timerCallback, BPS_TO_TIMER(100));
		callbackInstalled = true;
	}
	
	return timer;
}

LONG_LONG getTimerFrequency()
{
	return 100;
}
*/

#endif



