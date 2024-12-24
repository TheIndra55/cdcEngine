#include "Sound.h"

#include "srmusic.h"
#include "dynsfx.h"

#include "cdc/runtime/cdcSound/win32/snd.h"

void SOUND_UpdateSound(float time)
{
}

void SOUND_Init()
{
	cdc::SND_Init();

	SRMUSIC_Init();
	DYNSFX_Init();
	DYNSFX_UseBothReverbPools(1);

	cdc::SND_SetTimerFunc(SOUND_UpdateSound);
}

void SOUND_CreateGroups(SoundGroup* array, unsigned int count)
{
}