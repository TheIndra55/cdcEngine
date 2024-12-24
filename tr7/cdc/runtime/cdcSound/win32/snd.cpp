#include "snd.h"

#include <fmod.h>

#include "cdc/runtime/cdcSys/Assert.h"

static int totalsoundmem = 0;
static int numswchan = 0;

cdc::CHANNEL chans[64];

static void (*PCSoundTimerFunc)(float);

int cdc::SND_Init()
{
	numswchan = 64;

	if (!FSOUND_Init(44100, numswchan, 0))
	{
		FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
		FSOUND_Init(44100, numswchan, 0);
	}

	FSOUND_SetSFXMasterVolume(255);

	totalsoundmem = 0;

	for (int i = 0; i < numswchan; i++)
	{
		auto chan = &chans[i];

		chan->status = 0;
		chan->freq = 0;
		chan->sample = nullptr;
		chan->lvol = 0;
		chan->rvol = 0;
		chan->customplay = 0;
		chan->allocated = 0;
		chan->paused = 0;
		chan->priority = 0;
		chan->userdata = nullptr;
	}

	return 1;
}

void cdc::SND_Shutdown()
{
	PCSoundTimerFunc = nullptr;

	FSOUND_Close();
}

void cdc::SND_SetTimerFunc(void (*func)(float))
{
	PCSoundTimerFunc = func;
}

void cdc::SND_SystemTimer(float time)
{
	if (PCSoundTimerFunc)
	{
		PCSoundTimerFunc(time);
	}

	UpdateStatus();
}

void cdc::UpdateStatus()
{
	for (int i = 0; i < numswchan; i++)
	{
		auto chan = &chans[i];

		if (chan->status == 1 && FSOUND_IsPlaying(i))
		{
			chan->status = 0;

			if (chan->customplay)
			{
				chan->customplay = 0;
			}
			else
			{
				FSOUND_StopSound(i);
			}

			SND_FreeChannel(i);
		}
	}
}

void cdc::SND_FreeChannel(int channum)
{
	SND_Stop(channum);

	auto chan = &chans[channum];

	if (chan->allocated)
	{
		if (!chan->lostfunc)
		{
			cdc::FatalError("NULL lostfunc detected\n");
		}

		chan->lostfunc(channum);
		chan->lostfunc = nullptr;
		chan->allocated = 0;
	}
}

void cdc::SND_Stop(int channum)
{
	auto chan = &chans[channum];

	if (chan->status = 1)
	{
		chan->status = 0;

		if (chan->customplay)
		{
			chan->customplay = 0;
		}
		else
		{
			FSOUND_StopSound(channum);
		}

		SND_FreeChannel(channum);
	}
}

cdc::SAMPLE* cdc::SND_UploadSample(void* data, int len, int loopstart, int loopend)
{
	auto pcmLen = (len / 36) << 7;
	auto pcmData = (short*)new char[pcmLen];

	cdc::ADPCM_Decode((unsigned char*)data, pcmData, len / 36);

	auto sample = new SAMPLE;

	sample->len = pcmLen;
	sample->loopstart = loopstart;
	sample->loopend = loopend;

	totalsoundmem += pcmLen;

	sample->handle = FSOUND_Sample_Alloc(FSOUND_FREE, pcmLen >> 1, FSOUND_MONO | FSOUND_16BITS, 22100, 255, 128, 255);

	FSOUND_Sample_SetLoopPoints((FSOUND_SAMPLE*)sample->handle, loopstart, loopend);
	FSOUND_Sample_Upload((FSOUND_SAMPLE*)sample->handle, pcmData, FSOUND_MONO | FSOUND_16BITS);

	delete[] pcmData;

	return sample;
}