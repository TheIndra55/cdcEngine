#include "MultiplexStream.h"

#include <string.h>

#include "game/sound/sndapp.h"
#include <cdc/runtime/cdcSound/win32/snd.h>

MultiplexStreamImpl* MultiplexStreamImpl::s_activeStreamList = nullptr;

char MultiplexStreamImpl::s_soundStreamDir[256];
char MultiplexStreamImpl::s_cinematicStreamDir[256];

LPCRITICAL_SECTION MultiplexStreamImpl::s_critSection = nullptr;

MultiplexStream::MultiplexStream() : CinematicHandlerHost()
{
}

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

void MultiplexStream::SetMusicDirectory(const char* dir)
{
}

void MultiplexStream::SetBufferSize(int newSize)
{
}

MultiplexStream* MultiplexStream::CreateSoundStream(const char* name)
{
	auto msi = new MultiplexStreamImpl();

	if (msi->Init(kSoundStream, name))
	{
		return msi;
	}

	msi->Stop();
	msi->CleanUp();

	return nullptr;
}

MultiplexStream* MultiplexStream::CreateCinematicStream(const char* name)
{
	auto msi = new MultiplexStreamImpl();

	if (msi->Init(kCinematicStream, name))
	{
		return msi;
	}

	msi->Stop();
	msi->CleanUp();

	return nullptr;
}

MultiplexStream* MultiplexStream::CreateMusicStream(const char* name)
{
	auto msi = new MultiplexStreamImpl();

	if (msi->Init(kMusicStream, name))
	{
		return msi;
	}

	msi->Stop();
	msi->CleanUp();

	return nullptr;
}

MultiplexStreamImpl::MultiplexStreamImpl() : MultiplexStream()
{
	if (!s_critSection)
	{
		s_critSection = new CRITICAL_SECTION;
		InitializeCriticalSection(s_critSection);
	}

	if (s_activeStreamList)
	{
		s_activeStreamList->m_prev = this;
	}

	m_next = s_activeStreamList;
	m_prev = nullptr;
	s_activeStreamList = this;
}

bool MultiplexStreamImpl::Init(StreamType type, const char* name)
{
	char fileName[256];
	strcpy_s(fileName, name);

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

void MultiplexStreamImpl::Stop()
{
}

void MultiplexStreamImpl::Play()
{
}

bool MultiplexStreamImpl::IsPlaying()
{
	return false;
}

bool MultiplexStreamImpl::IsCueing()
{
	return false;
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
	int packetType, packetSize;

	while (pos < size)
	{
		switch (m_processState)
		{
		case ReadingPacketHeader:
			packetType = *(PacketType*)((char*)data + pos + 0);
			packetSize = *(int*)       ((char*)data + pos + 0);
			pos += 16;

			switch (packetType)
			{
			case kSoundPacket:
				m_processState = ReadingPacketInternalHeader;

				break;
			case kCinematicPacket:
				m_processState = ReadingCinematicPacket;
				m_packetBytesRemaining = packetSize;

				break;
			case kPaddingPacket:
				m_processState = ReadingPaddingPacket;
				m_packetBytesRemaining = packetSize;

				break;
			}

			break;
		case ReadingPacketInternalHeader:
			packetSize = *(int*)((char*)data + pos + 0);
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