#include "dynsfxld.h"
#include "dynsfx.h"

#include <string.h>

#include "cdc/runtime/cdcSys/Assert.h"
#include "cdc/runtime/cdcSound/win32/snd.h"

ResolveSection* CreateWaveSection()
{
	return new WaveSection();
}

unsigned int WaveSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	if (rtrID >= AAD_MAX_DYNAMIC_SFX_WAVES)
	{
		cdc::FatalError("Too many WAVES, %d >= AAD_MAX_DYNAMIC_SFX_WAVES", rtrID);
	}

	auto wave = &sfxPCWaveAttrTbl[rtrID];

	m_loader.buffer = nullptr;

	if (wave->referenceCount)
	{
		wave->referenceCount++;
		*skip = true;
	}
	else
	{
		wave->sampleData = nullptr;
		wave->sampleRate = 0;
		wave->smpLen = size - sizeof(Header);
		wave->referenceCount = 1;

		*skip = false;

		m_loader.state = STATE_BEGIN_HEADER;
	}

	return rtrID;
}

int WaveSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	auto wave = &sfxPCWaveAttrTbl[rtrID];

	auto ptr = pData;
	auto remaining = dataSize;

	while (true)
	{
		switch (m_loader.state)
		{
		case STATE_BEGIN_HEADER:
			m_loader.pHeader = &m_loader.header;
			m_loader.headerLen = sizeof(Header);
			m_loader.state = STATE_READ_HEADER;

		case STATE_READ_HEADER:
			if (remaining < m_loader.headerLen)
			{
				memcpy(m_loader.pHeader, ptr, remaining);

				m_loader.pHeader += remaining;
				m_loader.headerLen -= remaining;

				return dataSize - remaining;
			}

			memcpy(m_loader.pHeader, ptr, m_loader.headerLen);

			wave->sampleRate = m_loader.header.sampleRate;

			m_loader.loopStart = m_loader.header.loopStart;
			m_loader.loopEnd = m_loader.header.loopEnd;
			m_loader.state = STATE_READ_SAMPLE;
			m_loader.buffer = nullptr;
			m_loader.nBuffer = 0;

		case STATE_READ_SAMPLE:
			if (remaining >= wave->smpLen)
			{
				wave->sampleData = cdc::SND_UploadSample((void*)ptr, wave->smpLen, m_loader.loopStart, m_loader.loopEnd);
			}
		}
	}
}