#include "MultiplexStream.h"

#include <string.h>

#include "game/sound/sndapp.h"
#include <cdc/runtime/cdcSound/win32/snd.h>

char MultiplexStreamImpl::s_soundStreamDir[256];
char MultiplexStreamImpl::s_cinematicStreamDir[256];

void MultiplexStream::StopAllStreams()
{
}

bool MultiplexStream::InitSystem()
{
	cdc::ADPCM_CalcTable();

	return true;
}

void MultiplexStream::SetSoundDirectory(const char* dir)
{
	strcpy_s(MultiplexStreamImpl::s_soundStreamDir, dir);
}

void MultiplexStream::SetCinematicDirectory(const char* dir)
{
	strcpy_s(MultiplexStreamImpl::s_cinematicStreamDir, dir);
}

bool MultiplexStreamImpl::Init(StreamType type, char* name)
{
	char fileName[256];

	m_appHandle = SNDAPP_StreamOpenFile(fileName);

	if (!m_appHandle)
	{
		return false;
	}

	m_streamSize = SNDAPP_StreamGetSize(m_appHandle);
	m_streamType = type;
	m_fsoundStreamBufSize = 0;
	m_streamState = kReadingHeader;
	m_isWaitingForCine = true;

	SNDAPP_StreamRequestData(m_appHandle, 0, 2048, MultiplexStreamImpl::ReadHeaderCallback, this);

	return true;
}

void MultiplexStreamImpl::CleanUp()
{
	SNDAPP_StreamCloseFile(m_appHandle);
}

void MultiplexStreamImpl::ProcessSoundPacket(void* data, int size)
{
}

void MultiplexStreamImpl::ProcessCinematicPacket(void* data, int size)
{
}

void MultiplexStreamImpl::ProcessStream(void* data, int size)
{
	auto pos = 0;

	while (pos < size)
	{
		switch (m_processState)
		{
		case ReadingPacketHeader:
			auto packetType = *(PacketType*)((char*)data + pos + 0);
			auto packetSize = *(int*)((char*)data + pos + 0);
			pos += 16;

			switch (packetType)
			{
			case kSoundPacket:
				m_processState = ReadingPacketInternalHeader;

			case kCinematicPacket:
				m_processState = ReadingCinematicPacket;
				m_packetBytesRemaining = packetSize;

			case kPaddingPacket:
				m_processState = ReadingPaddingPacket;
				m_packetBytesRemaining = packetSize;

				break;
			}

			break;
		case ReadingPacketInternalHeader:
			auto packetSize = *(int*)((char*)data + pos + 0);
			pos += 16;

			m_packetBytesRemaining = packetSize;
			m_currentPacketSize = packetSize;

			m_processState = ReadingPacketData;

			break;
		case ReadingPacketData:
			ProcessSoundPacket((char*)data + pos, m_packetBytesRemaining);

			break;
		case ReadingCinematicPacket:
			ProcessCinematicPacket((char*)data + pos, m_packetBytesRemaining);

			break;
		case ReadingPaddingPacket:

			break;
		}
	}
}

void MultiplexStreamImpl::ReadHeaderCallback(void* data, int size, void* context)
{
	auto msi = (MultiplexStreamImpl*)context;

	msi->m_isWaitingForCine = false;

	if (msi->m_streamState == kDestroyed || msi->m_streamState == kCancelled)
	{
		msi->CleanUp();
		return;
	}

	memcpy(&msi->m_streamHeader, data, sizeof(STRHEADER));

	msi->m_channelCount = msi->m_streamHeader.channelCount;
	msi->m_curDataProcessedPos = 0;

	msi->m_currentStreamPosition = 2048;
	msi->m_streamState = kPrebuffering;
	msi->m_readingDataOffset = 0;
	msi->m_processState = ReadingPacketHeader;
	msi->m_isWaitingForCine = true;

	SNDAPP_StreamRequestData(msi->m_appHandle, msi->m_currentStreamPosition, 0, MultiplexStreamImpl::BufferCallback, context);
}

void MultiplexStreamImpl::BufferCallback(void* data, int size, void* context)
{
	auto msi = (MultiplexStreamImpl*)context;

	if (msi->m_streamState == kDestroyed || msi->m_streamState == kCancelled)
	{
		msi->CleanUp();
		return;
	}

	msi->ProcessStream(data, size);
}

void MultiplexStreamImpl::StreamCallback(void* data, int size, void* context)
{
}