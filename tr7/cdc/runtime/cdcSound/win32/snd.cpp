#include "snd.h"

static int totalsoundmem = 0;

cdc::SAMPLE* cdc::SND_UploadSample(void* data, int len, int loopstart, int loopend)
{
	auto dstLen = (len / 36) << 7;
	auto dst = (__int16*)new char[dstLen];

	cdc::ADPCM_Decode((unsigned char*)data, dst, len / 36);

	auto sample = new SAMPLE;

	sample->len = dstLen;
	sample->loopstart = loopstart;
	sample->loopend = loopend;

	totalsoundmem += dstLen;

	return sample;
}