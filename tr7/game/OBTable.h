#pragma once

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

void OBTABLE_Init();
void OBTABLE_ClearObjectReferences();
int OBTABLE_GetObjectID(char* name);