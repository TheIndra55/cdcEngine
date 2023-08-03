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
int OBTABLE_GetObjectID(char* name);