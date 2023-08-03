#pragma once

#include "game/resolve/Resolve.h"

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

struct Object
{
};

struct ObjectTracker
{
	ResolveObject* resolveObj;
	char* objectName;
	Object* object;
	__int16 objectID;
	__int16 objectStatus;
	__int16 numInUse;
	char numObjectsUsing;
	char objectsUsing[13];
	void* vramBlock;
};

void STREAM_Init();
ObjectTracker* STREAM_GetObjectTrackerByName(char* name);

int InsertGlobalObject(int id);