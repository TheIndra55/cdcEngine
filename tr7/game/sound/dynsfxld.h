#pragma once

#include "../resolve/ResolveSection.h"

class WaveSection : public ResolveSection
{
private:
	struct Header
	{
		unsigned int sampleRate;
		unsigned int loopStart;
		unsigned int loopEnd;
	};

	enum State
	{
		STATE_BEGIN_HEADER,
		STATE_READ_HEADER,
		STATE_READ_SAMPLE,
		STATE_SAMPLE_CREATE_FAILED
	};

	struct Loader
	{
		Header header;
		Header* pHeader;
		unsigned int headerLen;

		State state;

		unsigned int loopStart;
		unsigned int loopEnd;

		char* pSRAM;
		char* pUpload;
		char* buffer;
		int nBuffer;
	};

	Loader m_loader;

public:
	unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip);
	int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset);
};