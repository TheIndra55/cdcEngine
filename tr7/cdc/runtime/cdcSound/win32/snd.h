#pragma once

namespace cdc
{
	struct SAMPLE
	{
		unsigned int len;
		unsigned int loopstart;
		unsigned int loopend;

		void* handle;
	};

	struct CHANNEL
	{
		int used;
		int customplay;
		int allocated;
		int paused;
		int reverb;
		int priority;

		void(__cdecl* lostfunc)(int);
		void* userdata;

		int freq;
		SAMPLE* sample;
		int status;
		int loopaction;
		float lvol;
		float rvol;
	};

	int SND_Init();
	void SND_Shutdown();

	void SND_SetTimerFunc(void (*func)(float));
	void SND_SystemTimer(float time);

	void UpdateStatus();

	void SND_FreeChannel(int channum);
	void SND_Stop(int channum);

	SAMPLE* SND_UploadSample(void* data, int len, int loopstart, int loopend);

	void ADPCM_CalcTable();
	void ADPCM_Decode(unsigned char* pSrc, short* pDest, unsigned int numBlocks);
}