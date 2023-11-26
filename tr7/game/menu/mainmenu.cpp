#include <string.h>

#include "mainmenu.h"
#include <game/Main.h>

int gTitleUnitIndex;

void MAINMENU_SetTitleUnitForChapter(int chapterIndex)
{
	char testName[16];

	switch (chapterIndex)
	{
	case 0:
	case 1:
		strcpy(testName, "bolivia");
		break;

	case 3:
		strcpy(testName, "flashback");
		break;

	case 4:
	case 5:
		strcpy(testName, "tokyo");
		break;

	case 6:
		strcpy(testName, "amahlin");
		break;

	case 8:
		strcpy(testName, "russia");
		break;

	case 9:
		strcpy(testName, "grave");
		break;

	case 10:
	case 11:
		strcpy(testName, "himal");
		break;

	case 12:
		strcpy(testName, "boliviaredux");
		break;

	default:
		gTitleUnitIndex = 0;
		return;
	}

	auto titleScreen = globalInfo->titleScreen;

	if (titleScreen->numUnits <= 0)
	{
		return;
	}

	auto unitIndex = 0;

	for (int i = 0; i < titleScreen->numUnits; i++)
	{
		if (strstr(titleScreen->unitList[i].unitName, testName) != nullptr)
		{
			unitIndex = i;
			break;
		}
	}

	gTitleUnitIndex = unitIndex;
}