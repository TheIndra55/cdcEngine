#pragma once

#include "ResolveSection.h"

class ShaderLibSection : public ResolveSection
{
	unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip);
	int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset);
};