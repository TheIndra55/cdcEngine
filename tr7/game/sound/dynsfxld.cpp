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
	auto pWave = &sfxPCWaveAttrTbl[rtrID];

	auto dataOffset = 0u;
	auto bytesRemaining = dataSize;

	while (true)
	{
		switch (m_loader.state)
		{
		case STATE_BEGIN_HEADER:
			m_loader.pHeader = &m_loader.header;
			m_loader.headerLen = sizeof(Header);
			m_loader.state = STATE_READ_HEADER;

			break;
		case STATE_READ_HEADER:
			if (bytesRemaining < m_loader.headerLen)
			{
				memcpy(m_loader.pHeader, &pData[dataOffset], bytesRemaining);

				m_loader.pHeader = (Header*)((char*)m_loader.pHeader + bytesRemaining);
				m_loader.headerLen -= bytesRemaining;

				bytesRemaining = 0;

				return dataSize - bytesRemaining;
			}

			memcpy(m_loader.pHeader, &pData[dataOffset], m_loader.headerLen);

			bytesRemaining -= m_loader.headerLen;
			dataOffset += m_loader.headerLen;

			pWave->sampleRate = m_loader.header.sampleRate;

			m_loader.loopStart = m_loader.header.loopStart;
			m_loader.loopEnd = m_loader.header.loopEnd;
			m_loader.state = STATE_READ_SAMPLE;
			m_loader.buffer = nullptr;
			m_loader.nBuffer = 0;

			if (bytesRemaining == 0)
				return dataSize - bytesRemaining;

			break;
		case STATE_READ_SAMPLE:
			if (bytesRemaining >= pWave->smpLen)
			{
				pWave->sampleData = cdc::SND_UploadSample((void*)&pData[dataOffset], pWave->smpLen, m_loader.loopStart, m_loader.loopEnd);

				bytesRemaining -= pWave->smpLen;
				dataOffset += pWave->smpLen;

				if (bytesRemaining == 0)
					return dataSize - bytesRemaining;

				break;
			}

			return dataSize - bytesRemaining;
		case STATE_SAMPLE_CREATE_FAILED:
			return dataSize;
		}
	}
}