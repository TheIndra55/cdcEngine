#pragma once

struct STRHEADER
{
	int hertz;
	int startLoop;
	int endLoop;
	int channelCount;
	int reverbVol;
	int startSizeToLoad;
	int partialLoop;
	int loopAreaSize;
	int hasCinematic;
	int hasSubtitles;
	int loopStartFileOffset;
	int loopStartBundleOffset;
	int maxEEBytesPerRead;
	float mediaLength;
	float volLeft[12];
	float volRight[12];
	unsigned int loopStartSamplesToSkip[12];
};

enum StreamType
{
	kSoundStream,
	kCinematicStream,
	kMusicStream
};

enum StreamState
{
	kReadingHeader,
	kPrebuffering,
	kReadyToPlay,
	kPlaying,
	kFinished,
	kCancelled,
	kDestroyed
};

enum ProcessStreamState
{
	ReadingPacketHeader,
	ReadingPacketInternalHeader,
	ReadingPacketData,
	ReadingCinematicPacket,
	ReadingPaddingPacket
};

enum PacketType
{
	kSoundPacket,
	kCinematicPacket,
	kPaddingPacket
};

class MultiplexStream
{
public:
	static void StopAllStreams();

	static bool InitSystem();

	static void SetSoundDirectory(const char* dir);
	static void SetCinematicDirectory(const char* dir);
};

class MultiplexStreamImpl : public MultiplexStream
{
public:
	bool Init(StreamType type, char* name);

protected:
	void CleanUp();
	void ProcessSoundPacket(void* data, int size);
	void ProcessCinematicPacket(void* data, int size);
	void ProcessStream(void* data, int size);

	MultiplexStreamImpl* m_prev;
	MultiplexStreamImpl* m_next;

	int m_channelCount;

	StreamState m_streamState;

	int m_streamBufSize;
	int m_fsoundStreamBufSize;

	ProcessStreamState m_processState;

	int m_packetBytesRemaining;
	int m_readingDataOffset;
	int m_channelCurrentlyUpdating;
	int m_currentChannelOffset;
	int m_currentPacketSize;

	void* m_appHandle;

	StreamType m_streamType;
	int m_streamSize;
	STRHEADER m_streamHeader;
	int m_currentStreamPosition;

	unsigned int m_curDataProcessedPos;
	unsigned int m_curDataWrittenPos;

	bool m_isWaitingForCine;

	static void ReadHeaderCallback(void* data, int size, void* context);
	static void BufferCallback(void* data, int size, void* context);
	static void StreamCallback(void* data, int size, void* context);

	static char s_soundStreamDir[256];
	static char s_cinematicStreamDir[256];

	friend class MultiplexStream;
};