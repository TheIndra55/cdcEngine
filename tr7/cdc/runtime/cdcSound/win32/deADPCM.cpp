#include "snd.h"

#include <stdint.h>

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

static __int16 ADPCM_stepMul[16];
static unsigned int ADPCM_stepMulExp[256];

void cdc::ADPCM_CalcTable()
{
	for (int i = 0; i < 89; i++)
	{
		if (ADPCM_asStep[i] > 0x1FFF) ADPCM_asStep[i] = 0x1FFF;

		ADPCM_asStep[i] *= 4;
	}

	for (int i = 0; i < 16; i++)
	{
		auto delta = 0x800;

		if (i & 1) delta  = 0x1800;
		if (i & 2) delta += 0x2000;
		if (i & 4) delta += 0x4000;
		if (i & 8) delta  = -delta;

		ADPCM_stepMul[i] = delta;
	}

	for (int i = 0; i < 256; i += 4)
	{
		ADPCM_stepMulExp[i    ] = (uint16_t)ADPCM_stepMul[i       & 15] | ((uint16_t)ADPCM_stepMul[i       >> 4] << 16);
		ADPCM_stepMulExp[i + 1] = (uint16_t)ADPCM_stepMul[(i + 1) & 15] | ((uint16_t)ADPCM_stepMul[(i + 1) >> 4] << 16);
		ADPCM_stepMulExp[i + 2] = (uint16_t)ADPCM_stepMul[(i + 2) & 15] | ((uint16_t)ADPCM_stepMul[(i + 2) >> 4] << 16);
		ADPCM_stepMulExp[i + 3] = (uint16_t)ADPCM_stepMul[(i + 3) & 15] | ((uint16_t)ADPCM_stepMul[(i + 3) >> 4] << 16);
	}
}

void cdc::ADPCM_Decode(unsigned char* pSrc, __int16* pDest, unsigned int numBlocks)
{
	while (numBlocks--)
	{
		*pDest = *pSrc;

		auto step = ADPCM_asStep[pSrc[2]];
		auto delta = step >> 3;

		auto code = pSrc[4] >> 4;

		if (code & 1) delta += (step >> 2);
		if (code & 2) delta += (step >> 2);
		if (code & 4) delta += step;
		if (code & 8) delta  = -delta;

		if (delta > 0x8000) delta = 0x8000;
		if (delta < -0x8000) delta = -0x8000;

		pDest[1] = delta;

		for (int i = 0; i < 31; i++)
		{

		}
	}
}