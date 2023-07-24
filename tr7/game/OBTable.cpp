#include <stdio.h>
#include <Windows.h>

#include "OBTable.h"
#include "Main.h"

void OBTABLE_Init()
{
	char objectListFileName[80];
	sprintf(objectListFileName, "\\TR7\\%s\\objectlist.txt", ActiveBuildName);

	auto listText = FSHelper_ReadFile(objectListFileName, 0, GetFS());
}