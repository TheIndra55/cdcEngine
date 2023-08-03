#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Main.h"

#include "cdc/runtime/cdcSys/Assert.h" 
#include "cdc/runtime/cdcFile/FileSystem.h"
#include "cdc/runtime/cdcFile/MS/MSFileSystem.h"
#include "cdc/runtime/cdcFile/FileReceivers.h"

#include "game/archive/ArchiveFileSystem.h"
#include "game/pc/snd/MultiplexStream.h"
#include "game/Stream/stream.h"
#include "game/pc/D3D/RenderRA.h"
#include "game/pc/vram.h"
#include "game/Animation/anitracker.h"
#include "game/sound/Sound.h"

MainTracker mainTrackerX;

cdc::FileSystem* g_pDiskFS;
cdc::FileSystem* g_pFS;

char ActiveBuildName[16];
char ActiveBuildDir[32];

char startUnit[32];

cdc::FileSystem* GetFS()
{
	return g_pFS;
}

void InitFS()
{
	auto fileSystem = new cdc::MSFileSystem("");

	g_pDiskFS = fileSystem;
	g_pFS = fileSystem;
}

void InitArchive()
{
	auto fileSystem = new ArchiveFileSystem(g_pDiskFS);

	g_pFS = fileSystem;

	if (!fileSystem->Open("BIGFILE.DAT"))
	{
		cdc::FatalError("Unable to open bigfile BIGFILE.DAT!");
	}
}

bool MainG2()
{
	mainTrackerX.attractMovieName = nullptr;
	mainTrackerX.dontClearSave = false;

	GAMELOOP_SystemInit();

	SetupBuildDir("PC-W");

	if (ProcessArgs((char*)startUnit, &gameTrackerX))
	{
		return false;
	}

	mainTrackerX.mainState = 0;
	mainTrackerX.previousState = 0;
	mainTrackerX.done = 0;
	mainTrackerX.checkMemCardFull = false;
	mainTrackerX.checkMemCardNoCard = false;
	mainTrackerX.checkLevelComplete = false;

	RenderG2_SetBGColor(0.f, 0.f, 0.f);
	RenderG2_BeginScene(false);
	RenderG2_EndScene(frameRate_DefaultLocked);

	MAIN_DoMainInit();

	return false;
}

void MAIN_DoMainInit()
{
	STREAM_Init();
	VRAM_InitVramBlockCache();
	ANITRACKER_Init();
	SOUND_Init();

	MultiplexStream::InitSystem();

	char shaderDRMName[256];
	sprintf(shaderDRMName, "%s%s.drm", ActiveBuildDir, "shaders");
}

void SetupBuildDir(const char* configName)
{
	strcpy(ActiveBuildName, configName);
	sprintf(ActiveBuildDir, "%s\\", configName);

	char streamdir[256];
	char cinstreamdir[256];

	sprintf(streamdir, "%ssndstrm\\", ActiveBuildDir);
	sprintf(cinstreamdir, "%s", ActiveBuildDir);

	MultiplexStream::StopAllStreams();
	MultiplexStream::SetSoundDirectory(streamdir);
	MultiplexStream::SetCinematicDirectory(cinstreamdir);
}

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS)
{
	auto size = pFS->GetFileSize(fileName);

	if (!size)
	{
		return 0;
	}

	auto buffer = (char*)malloc(size + 1);

	if (!buffer)
	{
		return 0;
	}

	auto receiver = new cdc::FileUserBufferReceiver();
	receiver->m_pBuffer = buffer;

	auto request = pFS->RequestRead(receiver, fileName, 0);

	if (request)
	{
		request->Submit(cdc::FileRequest::HIGH);
		pFS->Synchronize();

		buffer[size] = 0;

		return buffer;
	}
	else
	{
		return 0;
	}
}

bool ProcessArgs(char* baseAreaName, GameTracker* gameTracker)
{
	return false;
}