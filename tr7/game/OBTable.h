#pragma once

#include "Stream/stream.h"

struct ObjectEntry
{
	char* name;
	int trackerIndex;
};

struct ObjectList
{
	int numObjects;
	ObjectEntry object[1];
};

extern ObjectList* gObjectList;

void OBTABLE_InitObjectWithID(Object* object);
void OBTABLE_InitAnimPointers(ObjectTracker* objectTracker);
void OBTABLE_Init();
void OBTABLE_ClearObjectReferences();
int OBTABLE_GetObjectID(char* name);