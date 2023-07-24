#pragma once

struct StreamUnitObjectList
{
	__int16 numObjects;
	unsigned __int16 objectList[1];
};

struct UnitInfo
{
	char name[16];
	StreamUnitObjectList* objectList;
};

struct StreamUnitList
{
	int numUnits;
	UnitInfo unit[1];
};

void STREAM_Init();