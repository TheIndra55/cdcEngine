#pragma once

#include "game/Stream/stream.h"
#include "game/Instance.h"
#include "game/sound/Sound.h"

struct menu_t
{
};

struct MemCardInfo
{
};

struct WipeInfo
{
};

struct MonsterAttackGroupData
{
};

struct SwarmObjectGroupData
{
};

struct VertexPool
{
};

struct CinematicHandlerHost;

struct GameTracker
{
	menu_t* menu;
	MemCardInfo* memcard;

	Level* level;
	Instance* playerInstance;

	VertexPool* vertexPool;

	int debugFlags;
	int debugFlags2;
	int debugFlags3;
	int debugFlags4;

	WipeInfo wipe[12];

	unsigned int displayFrameCount;
	unsigned int frameCount;
	int fpsFast;

	int gameFlags;
	int streamFlags;

	Level* mainDrawUnit;
	char baseAreaName[20];

	char levelDone;
	char levelChange;
	char loadingDisplayType;
	char gameMode;
	char cheatMode;
	char savingGame;

	__int16 postSaveScreenID;

	int StreamUnitID;

	char block_collide_override;
	char raziel_collide_override;
	char sound_collide_override;
	char attackwave_collide_override;

	int decoupleGame;

	unsigned int drawTime;
	unsigned int currentTime;
	unsigned int currentMaterialTime;
	unsigned int lastLoopTime;

	float oldtimeMult;
	int intoldtimeMult;

	unsigned int currentTicks;
	unsigned int totalTime;
	unsigned int idleTime;
	unsigned int CPUidleTime;

	int visibleInstances;
	int gameFramePassed;
	int timeSinceLastGameFrame;

	float timeMult;
	float unmodifiedActualTimeMult;
	float actualTimeMult;
	float globalTimeMult;
	float scrollProgress;
	float timeDilation;

	int debugTimeMult;

	float timeWarpDilation;
	float timeWarpDuration;
	float timeWarpRampTime;
	float timeWarpRemainingTime;

	int frameRateLock;

	char monster_collide_override;
	char monster_land_collide_override;
	char physics_line_draw_override;
	char inSuperActionCine;
	int noMonstersCount;

	unsigned __int16 monPCAware;
	unsigned __int16 monPCSaw;
	unsigned __int16 numMonsters;
	unsigned __int16 monSubset;
	unsigned __int16 chuckableBirthSubset;
	unsigned __int16 monBirthSubset;

	float monDistance;

	SoundStreamHolder streamHolder[2];
	MonsterAttackGroupData attackGroupData;
	SwarmObjectGroupData swarmGroupData;

	unsigned int musicVolume;
	unsigned int soundVolume;

	CinematicHandlerHost* activeCinematic;

	int playerMarkerObjectID;
	int currentPlayerObjectID;
	int altPlayerObjectID;

	float cameraTimeMult;
	float speedMultiplier;
	float targetSpeedMultiplier;
	float targetSpeedMultiplierInterpSpeed;

	int numBlockShift;

	bool drawBossMeters;
	int hardCodedHintType;
	float hardCodedHintTime;

	char darkChronicleCinematicStreamName[64];
	bool playingDarkChronicleCinematic;
	int currentDarkChronicle;
	bool darkChronicleFadeComplete;
	bool darkChronicleStartedFadeOut;

	int interpoBlendDebugLevel;
	unsigned __int64 playTime;
};

extern GameTracker gameTrackerX;

void GAMELOOP_AllocStaticMemory();
void GAMELOOP_InitGameTracker();
void GAMELOOP_SystemInit();
void GAMELOOP_ClearGameTracker();
void GAMELOOP_MainLoop(GameTracker* gameTracker);