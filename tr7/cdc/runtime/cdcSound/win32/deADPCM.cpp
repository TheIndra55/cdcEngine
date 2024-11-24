#include "snd.h"

#include <cstdint>

#include "cdc/runtime/cdcMath/Math.h"

static int ADPCM_asNextStep[16] = {
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};

static int ADPCM_asStep[89] = {
	7, 8, 9, 10, 11, 12, 13, 14,
	16, 17, 19, 21, 23, 25, 28, 31,
	34, 37, 41, 45, 50, 55, 60, 66,
	73, 80, 88, 97, 107, 118, 130, 143,
	157, 173, 190, 209, 230, 253, 279, 307,
	337, 371, 408, 449, 494, 544, 598, 658,
	724, 796, 876, 963, 1060, 1166, 1282, 1411,
	1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
	3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
	7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
	32767
};

void cdc::ADPCM_CalcTable()
{
	for (int i = 0; i < 89; i++)
	{
		if (ADPCM_asStep[i] > 0x1FFF) ADPCM_asStep[i] = 0x1FFF;
	}
}

static inline int DecodeSample(int code, int predicted, int step)
{
	auto delta = step >> 3;

	if (code & 1) delta += (step >> 2);
	if (code & 2) delta += (step >> 1);
	if (code & 4) delta += step;
	if (code & 8) delta  = -delta;

	delta += predicted;
	delta  = cdc::Clamp(delta, -32768, 32767);

	return delta;
}

static inline int NextStepIndex(int step, int code)
{
	step += ADPCM_asNextStep[code];
	step  = cdc::Clamp(step, 0, 88);

	return step;
}

void cdc::ADPCM_Decode(unsigned char* pSrc, short* pDest, unsigned int numBlocks)
{
	while (numBlocks--)
	{
		auto pBlock = pSrc;
		int code, step, index, delta;

		// Initial predictor
		*pDest = *(int16_t*)pBlock;

		// Sample 1
		code  = pBlock[4] >> 4;
		index = ADPCM_asStep[pBlock[2]];
		delta = DecodeSample(code, *(int16_t*)pBlock, index);
		step  = NextStepIndex(pBlock[2], code);

		pDest[1] = (int16_t)delta;

		for (int i = 0; i < 31; i++)
		{
			// Sample 1
			code  = pBlock[5] & 0xf;
			index = ADPCM_asStep[step];
			delta = DecodeSample(code, delta, index);
			step  = NextStepIndex(step, code);

			pDest[2] = (int16_t)delta;

			// Sample 2
			code  = pBlock[5] >> 4;
			index = ADPCM_asStep[step];
			delta = DecodeSample(code, delta, index);
			step  = NextStepIndex(step, code);

			pDest[3] = (int16_t)delta;

			pBlock++;
			pDest += 2;
		}

		pDest += 2;

		// Block alignment
		pSrc += 36;
	}
}