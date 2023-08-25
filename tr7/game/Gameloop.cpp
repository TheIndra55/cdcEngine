#include "Gameloop.h"

#include "Instance.h"
#include "miniInstance.h"
#include "OBTable.h"
#include "event.h"
#include "Save/SaveInfo.h"

GameTracker gameTrackerX;

VertexPool* gVertexPool;

__int64 currentTicks;

void GAMELOOP_AllocStaticMemory()
{
}

void GAMELOOP_InitGameTracker()
{
	gameTrackerX.vertexPool = gVertexPool;
	gameTrackerX.decoupleGame = 1;
	gameTrackerX.frameRateLock = 1;

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		GlobalObjects[i].resolveObj = nullptr;
	}

	gameTrackerX.savingGame = false;
	gameTrackerX.postSaveScreenID = 0;
	gameTrackerX.inSuperActionCine = false;
	gameTrackerX.debugTimeMult = 4096;
	gameTrackerX.cameraTimeMult = 1.f;
	gameTrackerX.timeMult = 1.f;
	gameTrackerX.unmodifiedActualTimeMult = 1.f;
	gameTrackerX.actualTimeMult = 1.f;
	gameTrackerX.timeDilation = 1.f;

	OBTABLE_ClearObjectReferences();
	EVENT_Init();
	INSTANCE_ClearAllSoundStreamHolders();

	gameTrackerX.hardCodedHintType = -1;
}

void GAMELOOP_SystemInit()
{
	GAMELOOP_AllocStaticMemory();

	INSTANCE_InitInstanceList();
	MINIINSTANCE_InitInstanceList();

	GAMELOOP_InitGameTracker();
}

void GAMELOOP_ClearGameTracker()
{
	if (GlobalSave)
	{
		gameTrackerX.currentTime = GlobalSave->header.currentTime;
		gameTrackerX.playTime = 1000000 * GlobalSave->header.playTimeSec;
	}
	else
	{
		gameTrackerX.currentTime = 0;
		gameTrackerX.playTime = 0ull;
	}

	gameTrackerX.currentMaterialTime = 0;
	gameTrackerX.currentTicks = 0;
	gameTrackerX.speedMultiplier = 1.f;

	currentTicks = 0ll;

	gameTrackerX.gameFlags = 0;
	gameTrackerX.frameCount = 0;
	gameTrackerX.fpsFast = 0;
	gameTrackerX.playerInstance = nullptr;
	gameTrackerX.activeCinematic = nullptr;
	gameTrackerX.playerMarkerObjectID = OBTABLE_GetObjectID("player");
}