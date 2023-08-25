#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

#include "game/Gameloop.h"
#include "game/sound/Sound.h"

struct MainTracker
{
	int mainState;
	int previousState;
	int movieNum;
	int done;
	int screen;
	const char* attractMovieName;
	bool dontClearSave;
	bool checkMemCardNoCard;
	bool checkMemCardFull;
	bool checkLevelComplete;
};

struct GlobalInfo
{
};

struct GlobalSoundInfo
{
	SoundEffects* soundEffects;

	int soundGroupCount;
	SoundGroup* soundGroup;
};

extern char ActiveBuildName[16];
extern char ActiveBuildDir[32];

cdc::FileSystem* GetFS();

void InitFS();
void InitArchive();

bool MainG2();
void MAIN_DoMainInit();

void SetupBuildDir(const char* configName);

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS);

void LOAD_ObjectFileName(char* name, char* object, char* extension);
void LOAD_UnitFileName(char* name, char* unit, char* extension);

bool ProcessArgs(char* baseAreaName, GameTracker* gameTracker);