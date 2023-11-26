#pragma once

struct CheckPoint
{
	char checkPointUnitName[20];
	int checkPointID;
};

struct SaveGameHeader
{
	__int16 flags;
	__int16 saveVersion;
	int sizeUsedInBlock;

	char percentComplete;
	char mansionPuzzleComplete;

	__int16 unlockedItemsCount;
	__int16 currentChapter;
	__int16 replayChapter;

	CheckPoint checkPoint;
	CheckPoint replayCheckPoint;
	CheckPoint mansionCheckPoint;

	unsigned int currentTime;
	unsigned int playTimeSec;
};

struct GlobalSaveTracker
{
	SaveGameHeader header;
};

extern GlobalSaveTracker* GlobalSave;

void SAVE_Init();