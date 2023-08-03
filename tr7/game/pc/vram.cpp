#include <string.h>

#include "vram.h"

unsigned __int64 TextureIDToTexHandle[0x2000];

void VRAM_InitVramBlockCache()
{
	memset(TextureIDToTexHandle, 0xFF, sizeof(TextureIDToTexHandle));
}