#include <string.h>

#include "vram.h"

unsigned __int64 TextureIDToTexHandle[0x2000];

void VRAM_InitVramBlockCache()
{
	memset(TextureIDToTexHandle, 0xFF, sizeof(TextureIDToTexHandle));
}

ResolveSection* CreateTextureSection()
{
	return new TextureSection();
}

unsigned int TextureSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = true;

	return 0;
}

int TextureSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	return dataSize;
}