#pragma once

#include <Windows.h>
#include <fmod.h>

#include "game/CinematicHandler.h"

#include "cdc/runtime/cdcMath/Vector.h"

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

class MultiplexStream : public CinematicHandlerHost
{
public:
	MultiplexStream();

	static void StopAllStreams();

	static bool InitSystem();

	static void SetSoundDirectory(const char* dir);
	static void SetCinematicDirectory(const char* dir);
	static void SetMusicDirectory(const char* dir);
	static void SetBufferSize(int newSize);

	static MultiplexStream* CreateSoundStream(const char* name);
	static MultiplexStream* CreateCinematicStream(const char* name);
	static MultiplexStream* CreateMusicStream(const char* name);

	virtual void Stop() = 0;
	virtual void Play() = 0;
	virtual bool IsPlaying() = 0;
	virtual bool IsCueing() = 0;
};

class MultiplexStreamImpl : public MultiplexStream
{
public:
	MultiplexStreamImpl();

	bool Init(StreamType type, const char* name);

	void Stop();
	void Play();
	bool IsPlaying();
	bool IsCueing();

protected:
	void CleanUp();
	void ProcessSoundPacket(void* data, int size);
	void ProcessCinematicPacket(void* data, int size);
	void ProcessStream(void* data, int size);

	MultiplexStreamImpl* m_prev;
	MultiplexStreamImpl* m_next;

	CinematicStream* m_cinematicStream;
	CinematicHandler* m_cinematicHandler;

	int m_channelCount;

	void* m_streamBuf[12];
	int m_channel[12];
	FSOUND_STREAM* m_fsoundStream[12];

	StreamState m_streamState;

	int m_streamBufSize;
	int m_fsoundStreamBufSize;

	ProcessStreamState m_processState;

	int m_packetBytesRemaining;
	int m_readingDataOffset;
	int m_channelCurrentlyUpdating;
	int m_currentChannelOffset;
	int m_currentPacketSize;

	char m_strname[128];

	void* m_appHandle;

	StreamType m_streamType;
	int m_streamSize;
	STRHEADER m_streamHeader;
	int m_currentStreamPosition;

	unsigned __int64 m_totalDataWritten;
	unsigned __int64 m_totalDataProcessed;

	unsigned int m_channelDataProcessedPos[12];
	unsigned int m_channelDataWrittenPos[12];
	unsigned int m_channelBufferUnderrunRefcount[12];

	unsigned int m_curDataProcessedPos;
	unsigned int m_curDataWrittenPos;

	float m_volumeLeft[12];
	float m_volumeRight[12];

	int m_group[12];

	int m_volumeReverb;
	float m_pitchMultiplier;

	bool m_useSoundVector;
	cdc::Vector4 m_soundVector;

	bool m_loadingBusy;
	bool m_isPlaying;
	bool m_isCueing;
	bool m_playRequest;
	bool m_isWaitingForCine;
	bool m_createdInPauseMode;

	int m_loopEnd;
	int m_loopPosition[12];

	int m_skipPacketBytes[12];
	int m_lastPacketStart;
	int m_loopFileOffset;
	int m_ignoreBytes;
	int m_loopPacketIgnore;

	__int64 m_bufferPlayBase[2];

	int m_curBufferBase;
	bool m_isLoopPacket;

	void* m_initThread;

	bool mCameraControl;
	bool mRestorePositions;
	int mEndTriggerID;

	static void ReadHeaderCallback(void* data, int size, void* context);
	static void BufferCallback(void* data, int size, void* context);
	static void StreamCallback(void* data, int size, void* context);

	static MultiplexStreamImpl* s_activeStreamList;

	static char s_soundStreamDir[256];
	static char s_cinematicStreamDir[256];

	static LPCRITICAL_SECTION s_critSection;

	friend class MultiplexStream;
};