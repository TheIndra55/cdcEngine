#pragma once

struct AadLoadedSfxWaveAttr
{
	unsigned int referenceCount;
	void* sampleData;
	unsigned int sampleRate;
	int smpLen;
};

#define AAD_MAX_DYNAMIC_SFX_WAVES 2048

extern AadLoadedSfxWaveAttr sfxPCWaveAttrTbl[AAD_MAX_DYNAMIC_SFX_WAVES];

void DYNSFX_StopAllSfx(bool bAllowRelease);