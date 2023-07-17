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

cdc::FileSystem* g_pDiskFS;
cdc::FileSystem* g_pFS;

char ActiveBuildName[16];
char ActiveBuildDir[32];

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