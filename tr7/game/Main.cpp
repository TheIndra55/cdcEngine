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
#include "game/font.h"
#include "game/debug.h"
#include "game/pc/gamewindow.h"
#include "game/Camera/Camera.h" 
#include "game/sound/dynsfx.h"
#include "game/menu/menu.h"
#include "game/Save/SaveInfo.h"
#include "game/menu/mainmenu.h"
#include "game/menu/UIItemImage.h"
#include "game/menu/UICommandManager.h"
#include "game/menu/UISystem.h"
#include "game/menu/UISystemGame.h"
#include "game/menu/UIFadeGroup.h"
#include "game/input/input.h"
#include "game/CdcPlannerRuntime.h"
#include "game/pc/movie/movie.h"

MainTracker mainTrackerX;

GlobalInfo* globalInfo;
GlobalSoundInfo* globalSoundInfo;

cdc::FileSystem* g_pDiskFS;
cdc::FileSystem* g_pFS;

char ActiveBuildName[16];
char ActiveBuildDir[32];

char startUnit[32];

int nosound;
unsigned int gRndSeed;
int DoMainMenu = 1;

static ResolveObject* s_pShaderResources;

InterfaceItem InterfaceItems[NUM_INTERFACE_ITEMS] =
{
	{
		"nvidia",			// name
		0,					// timeout
		0,					// buttonTimeout
		IFACE_ITEM_MOVIE,	// itemType
		TITLE_SCREEN,		// nextItem
		8.0,				// fadeUpSpeed
		8.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	},
	{
		"title",			// name
		0,					// timeout
		0,					// buttonTimeout
		IFACE_ITEM_MOVIE,	// itemType
		-1,					// nextItem
		8.0,				// fadeUpSpeed
		8.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	},
	{
		"legal",			// name
		300,				// timeout
		300,				// buttonTimeout
		IFACE_ITEM_SCREEN,	// itemType
		NVIDIA_SCREEN,		// nextItem
		16.0,				// fadeUpSpeed
		16.0,				// fadeDownSpeed
		kUIScreenFELegalID	// pushScreenID
	},
	{
		"credits",			// name
		0,					// timeout
		0,					// buttonTimeout
		IFACE_ITEM_MOVIE,	// itemType
		-1,					// nextItem
		16.0,				// fadeUpSpeed
		16.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	},
	{
		"theend_1",			// name
		300,				// timeout
		10,					// buttonTimeout
		IFACE_ITEM_SCREEN,	// itemType
		OUT2_SCREEN,		// nextItem
		16.0,				// fadeUpSpeed
		16.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	},
	{
		"theend_2",			// name
		0,					// timeout
		10,					// buttonTimeout
		IFACE_ITEM_SCREEN,	// itemType
		MOVIE_CREDITS,		// nextItem
		16.0,				// fadeUpSpeed
		16.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	},
	{
		"end",				// name
		0,					// timeout
		0,					// buttonTimeout
		IFACE_ITEM_MOVIE,	// itemType
		MOVIE_CREDITS,		// nextItem
		16.0,				// fadeUpSpeed
		16.0,				// fadeDownSpeed
		kUIScreenNoneID		// pushScreenID
	}
};

cdc::FileSystem* GetFS()
{
	return g_pFS;
}

void InitFS()
{
	auto fileSystem = new cdc::MSFileSystem("");

	g_pDiskFS = fileSystem;
	g_pFS = fileSystem;

	MOVIE_SetFileSystem(fileSystem);
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

bool MainG2_UpdateLoop()
{
	GAMEWINDOW_Poll();

	return false;
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

	DEBUG_Init();

	CdcPlannerInit();

	GAMEWINDOW_ReadIngameSettings();
	GAMEWINDOW_ApplyInGameSettings();

	strcpy(gameTrackerX.baseAreaName, startUnit);

	if (DoMainMenu)
	{
		mainTrackerX.movieNum = 2;
		mainTrackerX.mainState = InterfaceItems[0].itemType == IFACE_ITEM_SCREEN ? 7 : 3;
	}
	else
	{
		mainTrackerX.mainState = 2;
	}

	MainG2_UpdateLoop();

	while (true)
	{
		MainG2_UpdateLoop();

		RenderG2_BeginScene(true);
		RenderG2_EndScene(frameRate_DefaultLocked);
	}

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

	if (g_pFS->FileExists(shaderDRMName))
	{
		s_pShaderResources = Resolve::Load(shaderDRMName, nullptr, nullptr, nullptr, nullptr);
	}

	auto generalbank = STREAM_GetObjectTrackerByName("generalbank");
	auto globalsoundinfo = STREAM_GetObjectTrackerByName("globalsoundinfo");

	g_pFS->Synchronize();

	globalInfo = (GlobalInfo*)generalbank->object->data;
	globalSoundInfo = (GlobalSoundInfo*)globalsoundinfo->object->data;

	SOUND_CreateGroups(globalSoundInfo->soundGroup, globalSoundInfo->soundGroupCount);

	Font::Init();

	GAMELOOP_ClearGameTracker();

	CAMERA_Initialize(&theCamera);

	if (nosound)
	{
		DYNSFX_StopAllSfx(false);
	}

	MENU_Init(&gameTrackerX);
	SAVE_Init();

	gRndSeed = 0;

	MAINMENU_SetTitleUnitForChapter(0);

	UIImageManager::Init();
	UICommandManager::Init();
	UISystem::Init(new UISystemGame());
	UIScreenManager::DeregisterAllData();
	UIFadeGroupInit();

	INPUT_Init();
	GAMEWINDOW_RestoreInput();
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

	auto buffer = new char[size + 1];

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

void LOAD_ObjectFileName(char* name, char* object, char* extension)
{
	sprintf(name, "%s%s.%s", ActiveBuildDir, object, extension);
}

void LOAD_UnitFileName(char* name, char* unit, char* extension)
{
	sprintf(name, "%s%s.%s", ActiveBuildDir, unit, extension);
}

bool ProcessArgs(char* baseAreaName, GameTracker* gameTracker)
{
	return false;
}