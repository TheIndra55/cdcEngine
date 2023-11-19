#include "AnimationSection.h"

unsigned int AnimationSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = true;

	return 0;
}

int AnimationSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	return dataSize;
}