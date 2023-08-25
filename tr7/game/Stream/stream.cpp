#include <stdio.h>

#include "stream.h"

#include "cdc/runtime/cdcSys/Assert.h"

#include "game/OBTable.h"
#include "game/Main.h"

#define MAX_OBJECTS 94

StreamUnitList* gUnitList = nullptr;

ObjectTracker GlobalObjects[MAX_OBJECTS];

void STREAM_Init()
{
	if (!gUnitList)
	{
		OBTABLE_Init();

		auto fileSystem = GetFS();

		char objListPath[300];
		sprintf(objListPath, "\\TR7\\%s\\objlist.dat", ActiveBuildName);

		if (fileSystem->GetFileSize(objListPath) > 0)
		{
			gUnitList = (StreamUnitList*)FSHelper_ReadFile(objListPath, 0, fileSystem);
			auto objectList = (StreamUnitObjectList*)&gUnitList->unit[gUnitList->numUnits + 1];

			for (int i = 0; i < gUnitList->numUnits; i++)
			{
				auto unit = &gUnitList->unit[i];

				if (*unit->name)
				{
					unit->objectList = objectList;

					objectList++;
				}
			}
		}
		else
		{
			gUnitList = new StreamUnitList();
			gUnitList->numUnits = 0;
		}
	}
}

ObjectTracker* STREAM_GetObjectTrackerByName(char* name)
{
	auto id = OBTABLE_GetObjectID(name);

	if (!id)
	{
		cdc::FatalError("Unable to get ID for object: %s", name);
	}

	auto object = InsertGlobalObject(id);

	if (object == -1)
	{
		return nullptr;
	}

	return &GlobalObjects[object];
}

void STREAM_LoadObjectReturn(void* loadData, void* data, void* data2, ResolveObject* resolveObject)
{
	auto object = (Object*)loadData;

	auto dtpData = object->objectDTPData;
	if (dtpData && dtpData->OEDTemplateSelector)
	{
		object->data = dtpData->TuneDataPtr;
	}
}

int InsertGlobalObject(int id)
{
	auto name = gObjectList->object[id - 1].name;

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		auto object = &GlobalObjects[i];

		if (object->objectStatus && object->objectStatus != 5 && GlobalObjects->objectID == id)
		{
			return i;
		}
	}

	ObjectTracker* object;
	int i;
	for (i = 0; i < MAX_OBJECTS; i++)
	{
		object = &GlobalObjects[i];

		if (!object->objectStatus)
		{
			break;
		}
	}

	char string[256];
	LOAD_ObjectFileName(string, name, "drm");

	object->objectName = name;
	object->objectStatus = 1;
	object->objectID = id;
	object->resolveObj = Resolve::Load(string, STREAM_LoadObjectReturn, object, nullptr, (void**)&object->object);
	object->numInUse = 0;
	object->numObjectsUsing = 0;

	return i;
}