#include <stdio.h>
#include <Windows.h>

#include "OBTable.h"
#include "Main.h"

ObjectList* gObjectList;

void OBTABLE_InitObjectWithID(Object* object)
{
}

void OBTABLE_InitAnimPointers(ObjectTracker* objectTracker)
{
}

inline char* getline(char* str, char* text)
{
	if (*text != '\n')
	{
		while (*text != '\r')
		{
			*str++ = *text++;

			if (*text == '\n')
			{
				text++;
				break;
			}
		}
	}

	while (*text == '\n' || *text == '\r')
	{
		text++;
	}

	*str = 0;

	return text;
}

void OBTABLE_Init()
{
	char objectListFileName[80];
	sprintf(objectListFileName, "\\TR7\\%s\\objectlist.txt", ActiveBuildName);

	auto listText = FSHelper_ReadFile(objectListFileName, 0, GetFS());

	char str[512];
	listText = getline(str, listText);

	int numObjects;
	sscanf(str, "%d", &numObjects);

	char* start = listText;
	int largest = 0;

	for (int i = 0; i < numObjects; i++)
	{
		listText = getline(str, listText);

		int dummy;
		sscanf(str, "%d,", &dummy);

		if (dummy > largest)
		{
			largest = dummy;
		}
	}

	if (gObjectList)
	{
		free(gObjectList);
	}

	gObjectList = (ObjectList*)malloc(sizeof(ObjectList) + (largest * sizeof(ObjectEntry)) + (listText - start));
	gObjectList->numObjects = largest;

	for (int i = 0; i < largest; i++)
	{
		gObjectList->object[i].name = nullptr;
		gObjectList->object[i].trackerIndex = -1;
	}

	listText = start;
	char* names = (char*)&gObjectList->object[largest];

	for (int i = 0; i < numObjects; i++)
	{
		listText = getline(str, listText);

		ObjectEntry* object = &gObjectList->object[i];

		sscanf(str, "%d,%s", &object->trackerIndex, names);
		object->name = names;

		char* c = strchr(names, ',');
		if (c)
		{
			*c = 0;
		}

		names += strlen(names) + 1;
	}
}

void OBTABLE_ClearObjectReferences()
{
}

int OBTABLE_GetObjectID(char* name)
{
	if (gObjectList->numObjects <= 0)
	{
		return 0;
	}

	for (int i = 0; i < gObjectList->numObjects; i++)
	{
		if (strcmp(name, gObjectList->object[i].name) == 0)
		{
			return gObjectList->object[i].trackerIndex;
		}
	}

	return 0;
}