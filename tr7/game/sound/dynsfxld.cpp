#include "dynsfxld.h"

ResolveSection* CreateWaveSection()
{
	return new WaveSection();
}

unsigned int WaveSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = true;

	return 0;
}

int WaveSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	return dataSize;
}