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

	SAMPLE* SND_UploadSample(void* data, int len, int loopstart, int loopend);

	void ADPCM_CalcTable();
	void ADPCM_Decode(unsigned char* pSrc, __int16* pDest, unsigned int numBlocks);
}