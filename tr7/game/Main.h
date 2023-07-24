#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

#include "game/Gameloop.h"

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

extern char ActiveBuildName[16];
extern char ActiveBuildDir[32];

cdc::FileSystem* GetFS();

void InitFS();
void InitArchive();

bool MainG2();
void MAIN_DoMainInit();

void SetupBuildDir(const char* configName);

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS);
bool ProcessArgs(char* baseAreaName, GameTracker* gameTracker);